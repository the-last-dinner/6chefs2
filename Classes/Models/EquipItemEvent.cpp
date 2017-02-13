//
//  EquipItemEvent.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/13.
//
//

#include "Models/EquipItemEvent.h"

#include "Event/EventScript.h"
#include "Event/EventFactory.h"
#include "Event/GameEvent.h"
#include "Managers/DungeonSceneManager.h"
#include "Models/CommonEventScripts.h"
#include "Tasks/EventTask.h"

// 定数
const string EquipItemEvent::EVENT_SCRIPT_NAME {"EquipItemEvent"};

// 初期化
bool EquipItemEvent::init(CommonEventScripts* commonEventScripts)
{
    EventScript* equipItemEvent {commonEventScripts->getScript(EVENT_SCRIPT_NAME)};
    
    if (!equipItemEvent) return false;
    
    _equipItemEvent = equipItemEvent;
    
    this->setEquipmentCache(etoi(ItemID::UNDIFINED), etoi(ItemID::UNDIFINED));
    
    return true;
}

// 装備のキャッシュをリセット
void EquipItemEvent::setEquipmentCache(const int rightItemId, const int leftItemId)
{
    _rightItemIdCache = rightItemId;
    _leftItemIdCache = leftItemId;
}

// 装備状態でマップ移動時のイベント
void EquipItemEvent::updateEquipmentEvent()
{
    this->runCallbackEvent(CsvDataManager::getInstance()->getItemData()->getItemEquipUpdateEvent(_rightItemIdCache));
    this->runCallbackEvent(CsvDataManager::getInstance()->getItemData()->getItemEquipUpdateEvent(_leftItemIdCache));
}

// 装備変更時の処理
void EquipItemEvent::onChangeEquipment(const int rightItemId, const int leftItemId)
{
    // 解除イベント
    this->onReleaseItem(_rightItemIdCache, rightItemId, leftItemId);
    this->onReleaseItem(_leftItemIdCache, rightItemId, leftItemId);
    
    // 装備イベント
    this->onEquipItem(rightItemId);
    this->onEquipItem(leftItemId);
    
    // キャッシュを上書き
    this->setEquipmentCache(rightItemId, leftItemId);
}

// 装備解除時のイベント
void EquipItemEvent::onReleaseItem(const int targetCache, const int rightItemId, const int leftItemId)
{
    if (targetCache != etoi(ItemID::UNDIFINED) && targetCache != rightItemId && targetCache != leftItemId) {
        string eventName = CsvDataManager::getInstance()->getItemData()->getItemEquipEndEvent(targetCache);
        this->runCallbackEvent(eventName);
    }
}

// 装備時のイベント
void EquipItemEvent::onEquipItem(const int targetItemId)
{
    if (targetItemId > etoi(ItemID::UNDIFINED) && targetItemId != _rightItemIdCache && targetItemId != _leftItemIdCache) {
        string eventName = CsvDataManager::getInstance()->getItemData()->getItemEquipInitEvent(targetItemId);
        this->runCallbackEvent(eventName);
    }
}

// イベント実行
void EquipItemEvent::runCallbackEvent(const string &eventName)
{
    if (eventName == "") return;
    
    DungeonSceneManager* manager {DungeonSceneManager::getInstance()};
    EventFactory* factory {manager->getEventFactory()};
    rapidjson::Value eventJson {_equipItemEvent->getScriptJson(eventName)};
    GameEvent* event {factory->createGameEvent(eventJson, nullptr)};
    CC_SAFE_RETAIN(event);
    manager->getEventTask()->runEvent(event);
}
