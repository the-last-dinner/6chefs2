//
//  BattleEvent.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#include "Event/BattleEvent.h"

#include "Battle/Battle.h"
#include "Battle/BattleData.h"

#include "Event/EventScriptMember.h"
#include "Event/GameEventHelper.h"
#include "Event/EventFactory.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark BattleStartEvent

bool BattleStartEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::OBJECT_ID)) return false;
    if (!_eventHelper->hasMember(_json, member::TRUE_)) return false;
    if (!_eventHelper->hasMember(_json, member::FALSE_)) return false;
    
    BattleData* data { BattleData::create() };
    
    vector<int> targetIds {};
    rapidjson::Value& targetsJson { _json[member::OBJECT_ID] };
    for(int i { 0 }; i < targetsJson.Size(); i++) {
        targetIds.push_back(stoi(targetsJson[i].GetString()));
    }
    
    _successCallbackEvent = _factory->createGameEvent(_json[member::TRUE_], nullptr);
    _failureCallbackEvent = _factory->createGameEvent(_json[member::FALSE_], nullptr);
    
    data->setTargetObjectIds(targetIds);
    data->setSuccessCallbackEvent(_successCallbackEvent);
    data->setFailureCallbackEvent(_failureCallbackEvent);
    
    Battle* battle { Battle::create(data, DungeonSceneManager::getInstance()) };
    if (!battle) return false;
     
    _battle = battle;
    
    return true;
}

void BattleStartEvent::run()
{
    if (_successCallbackEvent) {
        _successCallbackEvent->setEventId(this->getEventId());
    }
    
    if (_failureCallbackEvent) {
        _failureCallbackEvent->setEventId(this->getEventId());
    }
    
    _battle->start();
    this->setDone();
}
