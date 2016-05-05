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

#pragma mark ShowLayerEvent

bool ShowLayerEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    this->layerName = json[member::LAYER].GetString();
    
    return true;
}

void ShowLayerEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getMapLayer()->showLayer(this->layerName);
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

#pragma mark -
#pragma mark QuakeMapEvent
bool QuakeMapEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    this->quakeLayer = static_cast<Node*>(DungeonSceneManager::getInstance()->getMapLayer()->getTiledMap());
    
    if (!this->validator->hasMember(json, member::TIME)) return false;
    this->time = json[member::TIME].GetDouble() / 4;
    
    if (this->validator->hasMember(json, member::X)) this->x = json[member::X].GetInt();
    if (this->validator->hasMember(json, member::Y)) this->y = json[member::Y].GetInt();

    return true;
}

void QuakeMapEvent::run()
{
    this->setDone();
    this->quakeLayer->runAction(Sequence::create
    (
        EaseCubicActionOut::create(MoveBy::create(this->time, Vec2(this->x,this->y))),
        EaseCubicActionOut::create(MoveBy::create(this->time, Vec2(-1 * this->x,-1 * this->y))),
        EaseCubicActionOut::create(MoveBy::create(this->time, Vec2(-1 * this->x,-1 * this->y))),
        EaseCubicActionOut::create(MoveBy::create(this->time, Vec2(this->x,this->y))),
        nullptr
    ));
}