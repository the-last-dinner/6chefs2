//
//  GameEventHelper.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/24.
//
//

#include "Event/GameEventHelper.h"

#include "Event/EventFactory.h"
#include "Event/EventScriptMember.h"
#include "Event/EventScript.h"
#include "Event/GameEvent.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/Character.h"
#include "MapObjects/Party.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
GameEventHelper::GameEventHelper() {};

// デストラクタ
GameEventHelper::~GameEventHelper() {};

// 初期化
bool GameEventHelper::init() { return true; }

// メンバーが存在するかどうか
bool GameEventHelper::hasMember(rapidjson::Value& json, const char* member) const
{
    return json.HasMember(member);
}

// condition情報からboolを算出
bool GameEventHelper::detectCondition(rapidjson::Value& json)
{
    if(!this->hasMember(json, member::CONDITION)) return false;
    
    map<string, bool(GameEventHelper::*)(rapidjson::Value&, bool)> pConditionFuncs
    {
        {"equip", &GameEventHelper::detectEquipFlg},
        {"event", &GameEventHelper::detectEventFlg},
        {"flg", &GameEventHelper::detectFlg},
        {"item", &GameEventHelper::detectItemFlg},
        {"status", &GameEventHelper::detectStatusFlg},
        {"trophy", &GameEventHelper::detectTrophyFlg},
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
bool GameEventHelper::detectEquipFlg(rapidjson::Value& json, bool negative)
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
bool GameEventHelper::detectEventFlg(rapidjson::Value& json, bool negative)
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
bool GameEventHelper::detectFlg(rapidjson::Value& json, bool negative)
{
    bool detection { false };
    
    
    if (!json.IsArray())
    {
        // 自分自身のステータスを確認
        detection = PlayerDataManager::getInstance()->getLocalData()->checkEventStatus(DungeonSceneManager::getInstance()->getLocation().map_id, DungeonSceneManager::getInstance()->getRunningEventId(), json.GetInt());
        if(negative) detection = !detection;
    }
    else if (!json[0].IsArray())
    {
        detection = PlayerDataManager::getInstance()->getLocalData()->checkEventStatus(stoi(json[0].GetString()), stoi(json[1].GetString()), json[2].GetInt());
        if(negative) detection = !detection;
    }
    else
    {
        // 複数の場合
        for(int i { 0 }; i < json.Size(); i++)
        {
            detection = PlayerDataManager::getInstance()->getLocalData()->checkEventStatus(stoi(json[i][0].GetString()), stoi(json[i][1].GetString()), json[i][2].GetInt());
            if(negative) detection = !detection;
            if(!detection) break;
        }
    }
    
    return detection;
}

// アイテム所持の確認
bool GameEventHelper::detectItemFlg(rapidjson::Value& json, bool negative)
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
bool GameEventHelper::detectStatusFlg(rapidjson::Value& json, bool negative)
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

// トロフィー所持確認
bool GameEventHelper::detectTrophyFlg(rapidjson::Value& json, bool negative)
{
    bool detection { false };
    
    // 複数のトロフィー
    if (json.IsArray())
    {
        for (int i { 0 }; i < json.Size(); i++)
        {
            detection = PlayerDataManager::getInstance()->getGlobalData()->hasTrophy(stoi(json[i].GetString()));
            if (negative) detection = !detection;
            if (!detection) break;
        }
    }
    else
    {
        detection = PlayerDataManager::getInstance()->getGlobalData()->hasTrophy(stoi(json.GetString()));
        if(negative) detection = !detection;
    }
    return detection;
}

// マップオブジェクトを取得
MapObject* GameEventHelper::getMapObjectById(const string& objectId, bool available)
{
    // heroであったら主人公を返す
    if (objectId == "hero" || objectId == etos(ObjectID::HERO))
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
Point GameEventHelper::getPoint(rapidjson::Value& json)
{
    if(!this->hasMember(json, member::X) || !this->hasMember(json, member::Y)) return Point::ZERO;
    
    return Point(json[member::X].GetInt(), json[member::Y].GetInt());
}

// nextX, nextYの組を取得
Point GameEventHelper::getToPoint(rapidjson::Value& json)
{
    if(!this->hasMember(json, member::NEXT_X) || !this->hasMember(json, member::NEXT_Y)) return Point::ZERO;
    
    return Point(json[member::NEXT_X].GetInt(), json[member::NEXT_Y].GetInt());
}

// 方向を取得
Direction GameEventHelper::getDirection(rapidjson::Value& json)
{
    if(!this->hasMember(json, member::DIRECTION)) return Direction::NONE;
    
    return Direction::convertString(json[member::DIRECTION].GetString());
}

// 敵の移動アルゴリズムの種類を取得
EnemyMovePattern GameEventHelper::getMovePatternForEnemy(rapidjson::Value& json)
{
    return this->hasMember(json, member::MOVE_PATTERN) ? static_cast<EnemyMovePattern>(stoi(json[member::MOVE_PATTERN].GetString())) : EnemyMovePattern::CHEAP_CHASER;
}

// 移動アルゴリズムの種類を取得
CharacterMovePattern GameEventHelper::getMovePatternForCharacter(rapidjson::Value& json)
{
    return this->hasMember(json, member::MOVE_PATTERN) ? static_cast<CharacterMovePattern>(stoi(json[member::MOVE_PATTERN].GetString())) : CharacterMovePattern::SIZE;
}

// トリガーを取得
Trigger GameEventHelper::getTrigger(rapidjson::Value& json)
{
    return this->hasMember(json, member::TRIGGER) ? static_cast<Trigger>(stoi(json[member::TRIGGER].GetString())):Trigger::SIZE;
}

// イベントIDを取得
int GameEventHelper::getEventId(rapidjson::Value& json)
{
    return this->hasMember(json, member::EVENT_ID)? stoi(json[member::EVENT_ID].GetString()):static_cast<int>(EventID::UNDIFINED);
}

// 色を取得
Color3B GameEventHelper::getColor(rapidjson::Value& json) const
{
    if(!this->hasMember(json, member::COLOR)) return Color3B::BLACK;
    
    rapidjson::Value& colorJson { json[member::COLOR] };
    return Color3B(colorJson[0].GetInt(), colorJson[1].GetInt(), colorJson[2].GetInt());
}

// ミニゲーム系イベントの成功コールバックイベントを生成
GameEvent* GameEventHelper::createMiniGameSuccessCallbackEvent(rapidjson::Value& json, EventFactory* factory, GameEvent* caller)
{
    if (!this->hasMember(json, member::TRUE_)) return nullptr;
    
    GameEvent* event { nullptr };
    
    if (json[member::TRUE_].IsString()) {
        rapidjson::Value eventJson { DungeonSceneManager::getInstance()->getEventScript()->getScriptJson(json[member::TRUE_].GetString()) };
        event = factory->createGameEvent(eventJson, nullptr);
        event->setEventId(stoi(json[member::TRUE_].GetString()));
    }
    
    if (json[member::TRUE_].IsArray()) event = factory->createGameEvent(json[member::TRUE_], caller);
    
    return event;
}

// ミニゲーム系イベントの失敗コールバックイベントを生成
GameEvent* GameEventHelper::createMiniGameFailureCallbackEvent(rapidjson::Value& json, EventFactory* factory, GameEvent* caller)
{
    if (!this->hasMember(json, member::FALSE_) && !this->hasMember(json, member::ACTION)) return nullptr;
    
    GameEvent* event { nullptr };
    
    if (json[member::FALSE_].IsString()) {
        rapidjson::Value eventJson { DungeonSceneManager::getInstance()->getEventScript()->getScriptJson(json[member::FALSE_].GetString()) };
        event = factory->createGameEvent(eventJson, nullptr);
        event->setEventId(stoi(json[member::FALSE_].GetString()));
    }
    
    if (json[member::FALSE_].IsArray()) event = factory->createGameEvent(json[member::FALSE_], caller);
    
    // NOTICE: これ不要？
    if (this->hasMember(json, member::ACTION)) {
        if (json[member::ACTION].IsString()) {
            rapidjson::Value eventJson { DungeonSceneManager::getInstance()->getEventScript()->getScriptJson(json[member::ACTION].GetString()) };
            event = factory->createGameEvent(eventJson, nullptr);
            event->setEventId(stoi(json[member::ACTION].GetString()));
        }
        
        if(json[member::ACTION].IsArray()) event = factory->createGameEvent(json[member::ACTION], caller);
    }
    
    return event;
}
