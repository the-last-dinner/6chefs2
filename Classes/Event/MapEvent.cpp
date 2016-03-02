//
//  MapEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/27.
//
//

#include "Event/MapEvent.h"

#include "Event/EventScriptMember.h"
#include "Event/EventScriptValidator.h"

#include "Layers/Dungeon/TiledMapLayer.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark HideLayerEvent

bool HideLayerEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    this->layerName = json[member::LAYER].GetString();
    
    return true;
}

void HideLayerEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getMapLayer()->hideLayer(this->layerName);
}

#pragma mark -
#pragma mark SwingLayerEvent

bool SwingLayerEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    this->layerName = json[member::LAYER].GetString();
    
    return true;
}

void SwingLayerEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getMapLayer()->swingLayer(this->layerName);
}

#pragma mark -
#pragma mark StopLayerActionEvent

bool StopLayerActionEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    return true;
}

void StopLayerActionEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getMapLayer()->stopLayerActions();
}