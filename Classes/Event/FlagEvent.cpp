//
//  FlagEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/27.
//
//

#include "Event/FlagEvent.h"

#include "Datas/Message/SystemMessageData.h"
#include "Event/EventScriptMember.h"
#include "Event/GameEventHelper.h"
#include "Layers/Message/SystemMessageLayer.h"
#include "Managers/DungeonSceneManager.h"
#include "Models/CommonEventScripts.h"
#include "Scenes/DungeonScene.h"
#include "Utils/AssertUtils.h"

#pragma mark NeverAgainEvent

bool NeverAgainEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init(json)) return false;
    
    return true;
}

void NeverAgainEvent::run()
{
    this->setDone();
    
    LocalPlayerData* localData { PlayerDataManager::getInstance()->getLocalData() };
    
    if (_eventHelper->hasMember(_json, member::EVENT)) {
        // 別イベントの指定がある場合
        if (_json[member::EVENT][0].IsArray()) {
            // 複数の場合
            int arr_size = _json[member::EVENT].Size();
            for (int i = 0; i < arr_size; i++) {
                localData->setEventNeverAgain(stoi(_json[member::EVENT][i][0].GetString()), stoi(_json[member::EVENT][i][1].GetString()));
            }
        } else {
            // 単数の場合
            localData->setEventNeverAgain(stoi(_json[member::EVENT][0].GetString()), stoi(_json[member::EVENT][1].GetString()));
        }
    } else {
        // 自分自身のイベントを設定
        localData->setEventNeverAgain(DungeonSceneManager::getInstance()->getLocation().map_id, this->getEventId());
    }
}

#pragma mark -
#pragma mark GetItemEvent

bool GetItemEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // アイテムIDを取得
    if (!_eventHelper->hasMember(_json, member::ITEM_ID)) return false;
    _itemId = stoi(_json[member::ITEM_ID].GetString());
    
    return true;
}

void GetItemEvent::run()
{
    PlayerDataManager::getInstance()->getLocalData()->setItem(_itemId);
    SoundManager::getInstance()->playSE(Resource::SE::GET_ITEM, 1.4f);
    DungeonSceneManager::getInstance()->getScene()->addChild(SystemMessageLayer::create(SystemMessageData::create(CsvDataManager::getInstance()->getItemData()->getItemName(_itemId) + "　を手に入れた"), [this]{this->setDone();}), Priority::SYSTEM_MESSAGE);
}

#pragma mark -
#pragma mark RemoveItemEvent

bool RemoveItemEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // アイテムIDを取得
    if (!_eventHelper->hasMember(_json, member::ITEM_ID)) return false;
    _itemId = stoi(_json[member::ITEM_ID].GetString());
    
    return true;
}

void RemoveItemEvent::run()
{
    this->setDone();
    bool isExist = PlayerDataManager::getInstance()->getLocalData()->removeItem(_itemId);
    // アイテムが存在しない場合
    if (!isExist) {
        LastSupper::AssertUtils::warningAssert("Warning: removeItemEvent\nitemID: "+ to_string(_itemId) +"\nこのアイテムは持っていません");
    }
}

#pragma mark -
#pragma mark AddProfileEvent

bool AddProfileEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // キャラクタIDを取得
    if (!_eventHelper->hasMember(_json, member::CHARA_ID)) return false;
    _charaId = stoi(_json[member::CHARA_ID].GetString());
    
    // 情報レベル
    if (!_eventHelper->hasMember(_json, member::INFO_ID)) return false;
    _infoLevel = stoi(_json[member::INFO_ID].GetString());
    
    return true;
}

void AddProfileEvent::run()
{
    this->setDone();
    PlayerDataManager::getInstance()->getLocalData()->setCharacterProfile(_charaId, _infoLevel);
}

#pragma mark -
#pragma mark ChangeChapterEvent

bool ChangeChapterEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // チャプターID
    if (!_eventHelper->hasMember(_json, member::CHAPTER_ID)) return false;
    _chapterId = stoi(_json[member::CHAPTER_ID].GetString());
    
    return true;
}

void ChangeChapterEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getCommonEventScriptsObject()->loadEventScripts(_chapterId);
    PlayerDataManager::getInstance()->getLocalData()->setChapterId(_chapterId);
}

#pragma mark -
#pragma mark ChangeLikabilityRatingEvent

bool ChangeLikabilityRatingEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // キャラクタID
    if (!_eventHelper->hasMember(_json, member::CHARA_ID)) return false;
    _charaId = stoi(json[member::CHARA_ID].GetString());
    
    // 好感度
    if (!_eventHelper->hasMember(_json, member::FAVORITE)) return false;
    _rating = stoi(json[member::FAVORITE].GetString());
    
    return true;
}

void ChangeLikabilityRatingEvent::run()
{
    this->setDone();
    PlayerDataManager::getInstance()->getLocalData()->setFriendship(_charaId, _rating);
}

#pragma mark -
#pragma mark ChangeEventStatus

bool ChangeEventStatusEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // status
    if (!_eventHelper->hasMember(_json, member::FLAG)) return false;
    if (!_json[member::FLAG].IsInt()) {
        LastSupper::AssertUtils::fatalAssert("ChangeEventStatusEvent(changeFlg)\nflgの型はintにしてください。");
        return false;
    }
    _status = _json[member::FLAG].GetInt();
    
    // map_id
    if (_eventHelper->hasMember(_json, member::MAP_ID)) {
        _mapId = stoi(_json[member::MAP_ID].GetString());
    }
    
    // event_id
    if (_eventHelper->hasMember(_json, member::EVENT_ID)) {
        _eventId = stoi(_json[member::EVENT_ID].GetString());
    }
    
    return true;
}

void ChangeEventStatusEvent::run()
{
    // map_id
    if (_mapId < 0) {
        _mapId = DungeonSceneManager::getInstance()->getLocation().map_id;
    }
    
    // event_id
    if (_eventId < 0) {
        _eventId = this->getEventId();
    }
    
    // 変更
    PlayerDataManager::getInstance()->getLocalData()->setEventStatus(_mapId, _eventId, _status);
    this->setDone();
}

#pragma mark -
#pragma mark GetTrophyEvent

bool GetTrophyEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // trophyId
    if (!_eventHelper->hasMember(_json, member::TROPHY_ID)) return false;
    _trophyId = stoi(_json[member::TROPHY_ID].GetString());
    
    return true;
}

void GetTrophyEvent::run()
{
    PlayerDataManager::getInstance()->getGlobalData()->setTrophy(_trophyId);
    this->setDone();
}

#pragma mark -
#pragma mark GiveItemEvent

bool GiveItemEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init(json)) return false;
    
    // toCharaId
    if (!_eventHelper->hasMember(_json, member::TO_CHARA_ID)) return false;
    _toCharaId = stoi(_json[member::TO_CHARA_ID].GetString());
    if (_toCharaId == etoi(CharacterID::UNDIFINED)) return false;
    
    // fromCharaId
    if (_eventHelper->hasMember(_json, member::FROM_CHARA_ID)) {
        _fromCharaId = stoi(_json[member::FROM_CHARA_ID].GetString());
    }
    
    return true;
}

void GiveItemEvent::run()
{
    LocalPlayerData* localPlayerData { PlayerDataManager::getInstance()->getLocalData() };
    if (_fromCharaId == etoi(CharacterID::UNDIFINED)) {
        _fromCharaId = localPlayerData->getMainCharaId();
    }
    
    vector<int> items { localPlayerData->getItemAll(_fromCharaId) };
    for (auto item : items) {
        localPlayerData->setItem(_toCharaId, item);
        localPlayerData->removeItem(_fromCharaId, item);
    }
    this->setDone();
}

#pragma mark -
#pragma mark SetGameClearEvent

bool SetGameClearEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    return true;
}

void SetGameClearEvent::run()
{
    this->setDone();
    PlayerDataManager::getInstance()->setGameClear();
}
