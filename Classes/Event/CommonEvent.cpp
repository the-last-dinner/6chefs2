//
//  CommonEvent.cpp
//  6chefs2
//
//  Created by S.inami on 2016/03/06.
//
//

#include "Event/CommonEvent.h"

#include "Event/EventScriptMember.h"
#include "Event/EventFactory.h"
#include "Event/EventScript.h"
#include "Scenes/DungeonScene.h"

#include "Tasks/EventTask.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark -
#pragma mark CommonEvent

bool CommonEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    int functionId {stoi((json[member::FUNCTION_ID].GetString()))};
    this->functionId = functionId;
    
    return true;
}

void CommonEvent::run()
{
    GameEvent* event = this->factory->createGameEvent(DungeonSceneManager::getInstance()->getCommonEventScript()->getScriptJson(this->functionId));
    CC_SAFE_RETAIN(event);
    DungeonSceneManager::getInstance()->pushEventFront(event);
    this->setDone();
}

