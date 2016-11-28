//
//  SceneEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#include "Event/EventFactory.h"
#include "Event/SceneEvent.h"

#include "Event/GameEventHelper.h"
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

#include "Utils/AssertUtils.h"

#pragma mark ChangeMapEvent

bool ChangeMapEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // 移動後に実行するイベントID
    if (_eventHelper->hasMember(_json, member::EVENT_ID)) _initEventId = stoi(_json[member::EVENT_ID].GetString());
    
    return true;
}

void ChangeMapEvent::run()
{
    Direction direction { Direction::NONE };
    
    // directionの指定がされている時
    if (_eventHelper->hasMember(_json, member::DIRECTION)) {
        direction = _eventHelper->getDirection(_json);
    } else {
        // directionが指定されていない時
        direction = DungeonSceneManager::getInstance()->getParty()->getMainCharacter()->getDirection();
    }
    
    Location destLocation { Location(stoi(_json[member::MAP_ID].GetString()), _json[member::X].GetInt(), _json[member::Y].GetInt(), direction) };
    Location currentLocation { DungeonSceneManager::getInstance()->getParty()->getMainCharacter()->getLocation() };
    
    this->setDone();
    DungeonSceneManager::getInstance()->changeMap(destLocation, currentLocation, _initEventId);
}

#pragma mark -
#pragma mark WaitEvent

bool WaitEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    _duration = { static_cast<float>(_json[member::TIME].GetDouble()) };
    
    return true;
}

void WaitEvent::run() {}

void WaitEvent::update(float delta)
{
    _duration -= delta;
    
    if (_duration <= 0) this->setDone();
}

#pragma mark -
#pragma mark FadeOutEvent

bool FadeOutEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    if (_eventHelper->hasMember(_json, member::TIME)) _duration = _json[member::TIME].GetDouble();
    
    _color = _eventHelper->getColor(_json);
    
    return true;
}

void FadeOutEvent::run()
{
    DungeonSceneManager::getInstance()->fadeOut(_color, _duration, [this]{this->setDone();});
}

#pragma mark -
#pragma mark FadeInEvent

bool FadeInEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    if (_eventHelper->hasMember(_json, member::TIME)) _duration = _json[member::TIME].GetDouble();
    
    return true;
}

void FadeInEvent::run()
{
    DungeonSceneManager::getInstance()->fadeIn(_duration, [this]{this->setDone();});
}

#pragma mark -
#pragma mark GameOverEvent

bool GameOverEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // ゲームオーバーのID
    if (_eventHelper->hasMember(_json, member::ID)) _gameOverId = stoi(_json[member::ID].GetString());
    
    return true;
}

void GameOverEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->exitDungeon(GameOverScene::create(static_cast<GameOverScene::Type>(_gameOverId)));
}

#pragma mark -
#pragma mark EndingEvent

bool EndingEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // エンディングID
    if (_eventHelper->hasMember(_json, member::ID)) _endingId = stoi(_json[member::ID].GetString());
    
    // エンディング終了時動作
    if (!_eventHelper->hasMember(_json, member::CALLBACK_)) return false;
    
    _event = _factory->createGameEvent(json[member::CALLBACK_], this);
    // @TODO: メモリリークしてそうなのでRELEASEすべきタイミングでリリースする。デストラクタだとまずそうなのでコールバック実行後とか？
    CC_SAFE_RETAIN(_event);
    return true;
}

void EndingEvent::run()
{
    EndingScene* ending { EndingScene::create(_endingId) };
    
    BaseScene* nowScene = (BaseScene*)Director::getInstance()->getRunningScene();
    ending->onEnterPushedScene = [nowScene](){
        nowScene->onEnterPushedScene();
    };
    ending->onExitPushedScene = [nowScene, this](){
        nowScene->onExitPushedScene();
        _isEnd = true;
    };
    SoundManager::getInstance()->stopBGMAll();
    Director::getInstance()->pushScene(ending);
    
}

void EndingEvent::update(float delta)
{
    if(_isEnd) {
        _event->run();
        this->setDone();
    }
}

#pragma mark -
#pragma mark BackToTitleEvent

bool BackToTitleEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    return true;
}

void BackToTitleEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->exitDungeon(TitleScene::create());
}

#pragma mark -
#pragma mark InfoAssertEvent

bool InfoAssertEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    _text = (_eventHelper->hasMember(_json, member::TEXT)) ? _json[member::TEXT].GetString() : "";
    
    return true;
}

void InfoAssertEvent::run()
{
    this->setDone();
    LastSupper::AssertUtils::infoAssert(_text);
}
