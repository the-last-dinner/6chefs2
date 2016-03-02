//
//  FlagEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/27.
//
//

#include "Event/FlagEvent.h"

#include "Event/EventScriptMember.h"
#include "Event/EventScriptValidator.h"

#include "Scenes/DungeonScene.h"

#include "Layers/Message/SystemMessageLayer.h"
#include "Datas/Message/SystemMessageData.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark NeverAgainEvent

bool NeverAgainEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    if(this->validator->hasMember(json, member::EVENT))
    {
        // 別イベントの指定がある場合
        if (json[member::EVENT][0].IsArray())
        {
            // 複数の場合
            int arr_size = json[member::EVENT].Size();
            for(int i = 0; i < arr_size; i++)
            {
                this->event.push_back(pair<int,int>({stoi(json[member::EVENT][i][0].GetString()), stoi(json[member::EVENT][i][1].GetString())}));
            }
        }
        else
        {
            // 単数の場合
            this->event.push_back(pair<int,int>({stoi(json[member::EVENT][0].GetString()), stoi(json[member::EVENT][1].GetString())}));
        }
    }
    else
    {
        // 自分自身のイベントを設定
        this->event.push_back(pair<int, int>({DungeonSceneManager::getInstance()->getLocation().map_id,DungeonSceneManager::getInstance()->getPushingEventid()}));
    }
    return true;
}

void NeverAgainEvent::run()
{
    this->setDone();
    int arr_size = this->event.size();
    for(int i = 0; i < arr_size; i++)
    {
        PlayerDataManager::getInstance()->getLocalData()->setEventNeverAgain(this->event[i].first, this->event[i].second);
    }
    
}

#pragma mark -
#pragma mark GetItemEvent

bool GetItemEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // アイテムIDを取得
    if(!this->validator->hasMember(json, member::ITEM_ID)) return false;
    this->itemId = stoi(json[member::ITEM_ID].GetString());
    
    return true;
}

void GetItemEvent::run()
{
    PlayerDataManager::getInstance()->getLocalData()->setItem(this->itemId);
    SoundManager::getInstance()->playSE(Resource::SE::GET_ITEM, 1.4f);
    DungeonSceneManager::getInstance()->getScene()->addChild(SystemMessageLayer::create(SystemMessageData::create(CsvDataManager::getInstance()->getItemName(this->itemId) + "　を手に入れた"), [this]{this->setDone();}), Priority::SYSTEM_MESSAGE);
}

#pragma mark -
#pragma mark RemoveItemEvent

bool RemoveItemEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // アイテムIDを取得
    if(!this->validator->hasMember(json, member::ITEM_ID)) return false;
    this->itemId = stoi(json[member::ITEM_ID].GetString());
    
    return true;
}

void RemoveItemEvent::run()
{
    this->setDone();
    PlayerDataManager::getInstance()->getLocalData()->removeItem(this->itemId);
}

#pragma mark -
#pragma mark AddProfileEvent

bool AddProfileEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // キャラクタIDを取得
    if(!this->validator->hasMember(json, member::CHARA_ID)) return false;
    this->charaId = stoi(json[member::CHARA_ID].GetString());
    
    // 情報レベル
    if(!this->validator->hasMember(json, member::INFO_ID)) return false;
    this->infoLevel = stoi(json[member::INFO_ID].GetString());
    
    return true;
}

void AddProfileEvent::run()
{
    this->setDone();
    PlayerDataManager::getInstance()->getLocalData()->setCharacterProfile(this->charaId, this->infoLevel);
}

#pragma mark -
#pragma mark ChangeChapterEvent

bool ChangeChapterEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // チャプターID
    if(!this->validator->hasMember(json, member::CHAPTER_ID)) return false;
    this->chapterId = stoi(json[member::CHAPTER_ID].GetString());
    
    return true;
}

void ChangeChapterEvent::run()
{
    this->setDone();
    PlayerDataManager::getInstance()->getLocalData()->setChapterId(this->chapterId);
}

#pragma mark -
#pragma mark ChangeLikabilityRatingEvent

bool ChangeLikabilityRatingEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // キャラクタID
    if(!this->validator->hasMember(json, member::CHARA_ID)) return false;
    this->charaId = stoi(json[member::CHARA_ID].GetString());
    
    // 好感度
    if(!this->validator->hasMember(json, member::FAVORITE)) return false;
    this->rating = stoi(json[member::FAVORITE].GetString());
    
    return true;
}

void ChangeLikabilityRatingEvent::run()
{
    this->setDone();
    PlayerDataManager::getInstance()->getLocalData()->setFriendship(this->charaId, this->rating);
}

#pragma mark -
#pragma mark ChangeEventStatus

bool ChangeEventStatusEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // status
    if(!this->validator->hasMember(json, member::FLAG)) return false;
    this->status = json[member::FLAG].GetInt();
    
    return true;
}

void ChangeEventStatusEvent::run()
{
    int map_id {DungeonSceneManager::getInstance()->getLocation().map_id};
    int event_id {DungeonSceneManager::getInstance()->getRunningEventId()};
    PlayerDataManager::getInstance()->getLocalData()->setEventStatus(map_id, event_id, this->status);
    this->setDone();
}
