//
//  EventScriptValidator.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/24.
//
//

#include "Event/EventScriptValidator.h"

#include "Event/EventScriptMember.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/Character.h"
#include "MapObjects/Party.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
EventScriptValidator::EventScriptValidator() {FUNCLOG};

// デストラクタ
EventScriptValidator::~EventScriptValidator() {FUNCLOG};

// 初期化
bool EventScriptValidator::init() {return true;}

// メンバーが存在するかどうか
bool EventScriptValidator::hasMember(rapidjson::Value& json, const char* member) const
{
    return json.HasMember(member);
}

// condition情報からboolを算出
bool EventScriptValidator::detectCondition(rapidjson::Value& json)
{
    if(!this->hasMember(json, member::CONDITION)) return false;
    
    map<string, bool(EventScriptValidator::*)(rapidjson::Value&, bool)> pConditionFuncs
    {
        {"equip", &EventScriptValidator::detectEquipFlg},
        {"event", &EventScriptValidator::detectEventFlg},
        {"flg", &EventScriptValidator::detectFlg},
        {"item", &EventScriptValidator::detectItemFlg},
        {"status", &EventScriptValidator::detectStatusFlg},
    };
    
    rapidjson::Value& conditions {json[member::CONDITION]};
    
    bool detection { false };

    for(int i { 0 }; i < conditions.Size(); i++)
    {
        for(rapidjson::Value::MemberIterator itr = conditions[i].MemberBegin(); itr != conditions[i].MemberEnd(); itr++)
        {
            string key {itr->name.GetString()};
            
            // typeを無視
            if(key == member::TYPE) continue;
            
            bool negative {false};
            
            if(key.find("N") == 0)
            {
                key = LastSupper::StringUtils::strReplace("N", "", key);
                negative = true;
            }
            
            // N判定
            detection = (this->*pConditionFuncs[key])(itr->value, negative);
            
            //ANDなのでfalseがあったらbreak;
            if(!detection) break;
        }
        //ORなのでtrueがあったらbreak;してreturn;
        if(detection) break;
    }
    return detection;
}

// 装備状態の確認
bool EventScriptValidator::detectEquipFlg(rapidjson::Value& json, bool negative)
{
    bool detection { false };

    for(int i { 0 }; i < json.Size(); i++)
    {
        detection = PlayerDataManager::getInstance()->getLocalData()->isEquipedItem(stoi(json[i].GetString()));
        if(negative) detection = !detection;
        if(!detection) break;
    }
    
    return detection;
}

// イベントを見たか確認
bool EventScriptValidator::detectEventFlg(rapidjson::Value& json, bool negative)
{
    bool detection { false };
    
    //複数のイベントの場合
    if(json[0].IsArray())
    {
        for(int i { 0 }; i < json.Size(); i++)
        {
            detection = PlayerDataManager::getInstance()->getLocalData()->checkEventIsDone(stoi(json[i][0].GetString()), stoi(json[i][1].GetString()));
            if(negative) detection = !detection;
            if(!detection) return false;
        }
        
    }
    //一つのイベントの場合
    else
    {
        detection = PlayerDataManager::getInstance()->getLocalData()->checkEventIsDone(stoi(json[0].GetString()), stoi(json[1].GetString()));
        if(negative) detection = !detection;
    }
    
    return detection;
}

// フラグの確認
bool EventScriptValidator::detectFlg(rapidjson::Value& json, bool negative)
{
    bool detection { false };
    
    // 現在のmap_idとevent_idを取得
    int map_id {DungeonSceneManager::getInstance()->getLocation().map_id};
    int event_id {DungeonSceneManager::getInstance()->getPushingEventid()};
    
    // 一つのイベントに対してステータスを確認
    detection = PlayerDataManager::getInstance()->getLocalData()->checkEventStatus(map_id, event_id, json.GetInt());
    if(negative) detection = !detection;
    
    return detection;
}

// アイテム所持の確認
bool EventScriptValidator::detectItemFlg(rapidjson::Value& json, bool negative)
{
    bool detection { false };
    
    // 複数の場合
    if(json.IsArray())
    {
        for(int i { 0 }; i < json.Size(); i++)
        {
            detection = PlayerDataManager::getInstance()->getLocalData()->hasItem(stoi(json[i].GetString()));
            if(negative) detection = !detection;
            if(!detection) break;
        }
    }
    // 一つの場合
    else
    {
        detection = PlayerDataManager::getInstance()->getLocalData()->hasItem(stoi(json.GetString()));
        if(negative) detection = !detection;
    }
    
    return detection;
}

// 好感度の確認
bool EventScriptValidator::detectStatusFlg(rapidjson::Value& json, bool negative)
{
    bool detection { false };
    
    //複数の好感度
    if(json[0].IsArray())
    {
        for(int i { 0 }; i < json.Size(); i++)
        {
            detection = PlayerDataManager::getInstance()->getLocalData()->checkFriendship(stoi(json[i][0].GetString()), stoi(json[i][1].GetString()));
            if(negative) detection = !detection;
            if(!detection) break;
        }
    }
    // 一つの時
    else
    {
        detection = PlayerDataManager::getInstance()->getLocalData()->checkFriendship(stoi(json[0].GetString()), stoi(json[1].GetString()));
        if(negative) detection = !detection;
    }
    
    return detection;
}

// マップオブジェクトを取得
MapObject* EventScriptValidator::getMapObjectById(const string& objectId, bool available)
{
    // heroであったら主人公を返す
    if (objectId == "hero")
    {
        return DungeonSceneManager::getInstance()->getParty()->getMainCharacter();
    }
    // heroでなければIDから検索して返す
    else
    {
        if(available) return DungeonSceneManager::getInstance()->getMapObjectList()->getMapObject(stoi(objectId));
        return DungeonSceneManager::getInstance()->getMapObjectList()->getMapObjectFromDisableList(stoi(objectId));
    }
}

// x,yの組を取得
Point EventScriptValidator::getPoint(rapidjson::Value& json)
{
    if(!this->hasMember(json, member::X) || !this->hasMember(json, member::Y)) return Point::ZERO;
    
    return Point(json[member::X].GetInt(), json[member::Y].GetInt());
}

// nextX, nextYの組を取得
Point EventScriptValidator::getToPoint(rapidjson::Value& json)
{
    if(!this->hasMember(json, member::NEXT_X) || !this->hasMember(json, member::NEXT_Y)) return Point::ZERO;
    
    return Point(json[member::NEXT_X].GetInt(), json[member::NEXT_Y].GetInt());
}

// 方向を取得
Direction EventScriptValidator::getDirection(rapidjson::Value& json)
{
    if(!this->hasMember(json, member::DIRECTION)) return Direction::SIZE;
    
    return MapUtils::toEnumDirection(json[member::DIRECTION].GetString());
}

// 敵の移動アルゴリズムの種類を取得
EnemyMovePattern EventScriptValidator::getMovePatternForEnemy(rapidjson::Value& json)
{
    return this->hasMember(json, member::MOVE_PATTERN) ? static_cast<EnemyMovePattern>(stoi(json[member::MOVE_PATTERN].GetString())) : EnemyMovePattern::CHEAP_CHASER;
}

// 色を取得
Color3B EventScriptValidator::getColor(rapidjson::Value& json) const
{
    if(!this->hasMember(json, member::COLOR)) return Color3B::BLACK;
    
    rapidjson::Value& colorJson { json[member::COLOR] };
    return Color3B(colorJson[0].GetInt(), colorJson[1].GetInt(), colorJson[2].GetInt());
}
