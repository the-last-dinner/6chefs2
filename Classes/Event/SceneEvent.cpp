//
//  SceneEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#include "Event/EventFactory.h"
#include "Event/SceneEvent.h"

#include "Event/EventScriptValidator.h"
#include "Event/EventScriptMember.h"

#include "Datas/Scene/DungeonCameraSceneData.h"
#include "Datas/Scene/DungeonSceneData.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/Character.h"
#include "MapObjects/Party.h"

#include "Managers/DungeonSceneManager.h"
#include "Models/StopWatch.h"

#include "Scenes/EndingScene.h"
#include "Scenes/DungeonCameraScene.h"
#include "Scenes/DungeonScene.h"
#include "Scenes/GameOverScene.h"
#include "Scenes/TitleScene.h"

#pragma mark ChangeMapEvent

bool ChangeMapEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    Direction direction {Direction::SIZE};

    // directionの指定がされている時
    if(this->validator->hasMember(json, member::DIRECTION))
    {
        direction = MapUtils::toEnumDirection(json[member::DIRECTION].GetString());
    }
    // directionが指定されていない時
    else
    {
        direction = DungeonSceneManager::getInstance()->getParty()->getMainCharacter()->getDirection();
    }
    
    this->destLocation = Location(stoi(json[member::MAP_ID].GetString()), json[member::X].GetInt(), json[member::Y].GetInt(), direction);
    this->currentLocation = DungeonSceneManager::getInstance()->getParty()->getMainCharacter()->getLocation();
    
    // 移動後に実行するイベントID
    if(this->validator->hasMember(json, member::EVENT_ID)) this->initEventId = stoi(json[member::EVENT_ID].GetString());
    
    return true;
}

void ChangeMapEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->changeMap(this->destLocation, this->currentLocation, this->initEventId);
}

#pragma mark -
#pragma mark CreateCameraEvent

bool CreateCameraEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // 映したい場所
    this->location.map_id = (this->validator->hasMember(json, member::MAP_ID)) ? stoi(json[member::MAP_ID].GetString()) : DungeonSceneManager::getInstance()->getLocation().map_id;
 
    Point position { this->validator->getPoint(json) };
    this->location.x = position.x;
    this->location.y = position.y;
    
    // ターゲット
    if(this->validator->hasMember(json, member::OBJECT_ID)) this->objId = stoi(json[member::OBJECT_ID].GetString());
    
    // イベント
    if(!this->validator->hasMember(json, member::ACTION)) return false;
    this->event = this->factory->createGameEvent(json[member::ACTION]);
    CC_SAFE_RETAIN(this->event);
    
    return true;
}

void CreateCameraEvent::run()
{
    DungeonCameraSceneData* data { DungeonCameraSceneData::create(this->location) };
    data->setTargetId(this->objId);
    
    DungeonCameraScene* scene { DungeonCameraScene::create(data, this->event, [this]{DungeonSceneManager::getInstance()->popCameraScene(); this->setDone();}) };
    DungeonSceneManager::getInstance()->pushCameraScene(scene);
}

#pragma mark -
#pragma mark MoveCameraEvent

bool MoveCameraEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // 目的地
    this->toPosition = this->validator->getToPoint(json);
    
    // 移動時間
    if(this->validator->hasMember(json, member::TIME)) this->duration = json[member::TIME].GetDouble();
    
    return true;
}

void MoveCameraEvent::run()
{
    DungeonSceneManager::getInstance()->moveCamera(this->toPosition, this->duration, [this]{this->setDone();});
}

#pragma mark -
#pragma mark WaitEvent

bool WaitEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    this->duration = {static_cast<float>(json[member::TIME].GetDouble())};;
    
    return true;
}

void WaitEvent::run() {}

void WaitEvent::update(float delta)
{
    this->duration -= delta;
    
    if(this->duration <= 0) this->setDone();
}

#pragma mark -
#pragma mark FadeOutEvent

bool FadeOutEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    if(this->validator->hasMember(json, member::TIME)) this->duration = json[member::TIME].GetDouble();
    
    this->color = this->validator->getColor(json);
    
    return true;
}

void FadeOutEvent::run()
{
    DungeonSceneManager::getInstance()->fadeOut(this->color, this->duration, [this]{this->setDone();});
}

#pragma mark -
#pragma mark FadeInEvent

bool FadeInEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    if(this->validator->hasMember(json, member::TIME)) this->duration = json[member::TIME].GetDouble();
    
    return true;
}

void FadeInEvent::run()
{
    DungeonSceneManager::getInstance()->fadeIn(this->duration, [this]{this->setDone();});
}

#pragma mark -
#pragma mark GameOverEvent

bool GameOverEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // ゲームオーバーのID
    if(this->validator->hasMember(json, member::ID)) this->gameOverId = stoi(json[member::ID].GetString());
    
    return true;
}

void GameOverEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->exitDungeon(GameOverScene::create(static_cast<GameOverScene::Type>(this->gameOverId)));
}

#pragma mark -
#pragma mark EndingEvent

bool EndingEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // エンディングID
    if(this->validator->hasMember(json, member::ID)) this->endingId = stoi(json[member::ID].GetString());
    
    return true;
}

void EndingEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->exitDungeon(EndingScene::create(this->endingId));
}

#pragma mark -
#pragma mark BackToTitleEvent

bool BackToTitleEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    return true;
}

void BackToTitleEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->exitDungeon(TitleScene::create());
}
