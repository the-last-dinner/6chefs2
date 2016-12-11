//
//  CameraEvent.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/10/03.
//
//

#include "Event/CameraEvent.h"

#include "Event/GameEventHelper.h"
#include "Event/EventFactory.h"
#include "Event/EventScriptMember.h"

#include "Datas/Scene/DungeonCameraSceneData.h"
#include "Datas/Scene/DungeonSceneData.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/Character.h"
#include "MapObjects/Party.h"
#include "Managers/DungeonSceneManager.h"

#include "Scenes/DungeonCameraScene.h"
#include "Scenes/DungeonScene.h"

#include "Tasks/CameraTask.h"

#include "Utils/AssertUtils.h"

#pragma mark -
#pragma mark CreateCameraEvent

bool CreateCameraEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // 映したい場所
    if (_eventHelper->hasMember(_json, member::MAP_ID)) {
        _location.map_id = stoi(_json[member::MAP_ID].GetString());
    } else {
        _location.map_id = DungeonSceneManager::getInstance()->getLocation().map_id;
    }
    
    Point position { _eventHelper->getPoint(_json) };
    _location.x = position.x;
    _location.y = position.y;
    
    // ターゲット
    if (_eventHelper->hasMember(_json, member::OBJECT_ID)) _objId = stoi(_json[member::OBJECT_ID].GetString());
    
    // イベント
    if (!_eventHelper->hasMember(_json, member::ACTION)) return false;
    _eventJson = _json[member::ACTION];
    
    return true;
}

void CreateCameraEvent::run()
{
    DungeonCameraSceneData* data { DungeonCameraSceneData::create(_location) };
    data->setTargetId(_objId);
    data->setEventJson(_eventJson);
    
    DungeonCameraScene* scene { DungeonCameraScene::create(data, [this] {
        DungeonSceneManager::getInstance()->popCameraScene();
        this->setDone();
    }) };
    
    DungeonSceneManager::getInstance()->pushCameraScene(scene);
}

#pragma mark -
#pragma mark MoveCameraEvent

bool MoveCameraEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // 目的地
    _toPosition = _eventHelper->getToPoint(_json);
    
    // 移動時間
    if (_eventHelper->hasMember(_json, member::TIME)) _duration = _json[member::TIME].GetDouble();
    
    return true;
}

void MoveCameraEvent::run()
{
    DungeonSceneManager::getInstance()->getCamera()->move(_toPosition, _duration, [this]{ this->setDone(); });
}

#pragma mark -
#pragma mark SetCameraTargetEvent

bool SetCameraTargetEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    if (_eventHelper->hasMember(_json, member::OBJECT_ID)) {
        _objectId = stoi(_json[member::OBJECT_ID].GetString());
    } else {
        _objectId = etoi(ObjectID::HERO);
    }
    
    return true;
}

void SetCameraTargetEvent::run()
{
    MapObject* target { _eventHelper->getMapObjectById(to_string(_objectId)) };
    CameraTask* camera { DungeonSceneManager::getInstance()->getCamera() };
    camera->setTarget(target);
    camera->resumeFollowing();
    
    this->setDone();
}
