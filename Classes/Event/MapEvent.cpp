//
//  MapEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/27.
//
//

#include "Event/MapEvent.h"

#include "Event/EventScriptMember.h"
#include "Event/GameEventHelper.h"

#include "Layers/Dungeon/TiledMapLayer.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark HideLayerEvent

bool HideLayerEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    _layerName = _json[member::LAYER].GetString();
    
    return true;
}

void HideLayerEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getMapLayer()->hideLayer(_layerName);
}

#pragma mark ShowLayerEvent

bool ShowLayerEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    _layerName = _json[member::LAYER].GetString();
    
    return true;
}

void ShowLayerEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getMapLayer()->showLayer(_layerName);
}

#pragma mark -
#pragma mark SwingLayerEvent

bool SwingLayerEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    _layerName = _json[member::LAYER].GetString();
    
    return true;
}

void SwingLayerEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getMapLayer()->swingLayer(_layerName);
}

#pragma mark -
#pragma mark StopLayerActionEvent

bool StopLayerActionEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init(json)) return false;
    
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
    if(!GameEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::TIME)) return false;
    _time = _json[member::TIME].GetDouble() / 4;
    
    if (_eventHelper->hasMember(_json, member::X)) _x = _json[member::X].GetInt();
    if (_eventHelper->hasMember(_json, member::Y)) _y = _json[member::Y].GetInt();

    return true;
}

void QuakeMapEvent::run()
{
    this->setDone();
    Node* target { DungeonSceneManager::getInstance()->getMapLayer()->getTiledMap() };
    target->runAction(Sequence::create
    (
        EaseCubicActionOut::create(MoveBy::create(_time, Vec2(_x,_y))),
        EaseCubicActionOut::create(MoveBy::create(_time, Vec2(-1 * _x, -1 * _y))),
        EaseCubicActionOut::create(MoveBy::create(_time, Vec2(-1 * _x, -1 * _y))),
        EaseCubicActionOut::create(MoveBy::create(_time, Vec2(_x, _y))),
        nullptr
    ));
}
