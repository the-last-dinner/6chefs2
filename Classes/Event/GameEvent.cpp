//
//  GameEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/23.
//
//

#include "Event/GameEvent.h"

#include "Event/EventScript.h"
#include "Event/EventFactory.h"
#include "Event/GameEventHelper.h"
#include "Event/EventScriptMember.h"

#include "Managers/DungeonSceneManager.h"
#include "Models/CommonEventScripts.h"

#pragma mark GameEvent

//コンストラクタ
GameEvent::GameEvent() {};

// デストラクタ
GameEvent::~GameEvent()
{
    CC_SAFE_RELEASE_NULL(this->factory);
    CC_SAFE_RELEASE_NULL(this->eventHelper);
};

// 初期化
bool GameEvent::init()
{
    EventFactory* factory {DungeonSceneManager::getInstance()->getEventFactory()};
    GameEventHelper* eventHelper {DungeonSceneManager::getInstance()->getGameEventHelper()};
    
    if(!factory || !eventHelper) return false;
    
    CC_SAFE_RETAIN(factory);
    this->factory = factory;
    
    CC_SAFE_RETAIN(eventHelper);
    this->eventHelper = eventHelper;
    
    return true;
}

// インスタンスが再利用可能か
bool GameEvent::isReusable() const
{
    return this->_isReusable;
}

// 再利用可能か設定
void GameEvent::setReusable(bool reusable)
{
    this->_isReusable = reusable;
}

// イベントが終了しているか
bool GameEvent::isDone() const
{
    return this->_isDone;
}

// イベントを終了状態にする
void GameEvent::setDone(bool done)
{
    this->_isDone = done;
}

// EventIDもしくはaction配列からイベントを生成
GameEvent* GameEvent::createSpawnFromIdOrAction(rapidjson::Value& json)
{
    // eventIDの指定があれば、指定のIDに対応するjsonから生成
    if(this->eventHelper->hasMember(json, member::EVENT_ID))
    {
        return this->factory->createGameEvent(DungeonSceneManager::getInstance()->getEventScript()->getScriptJson(json[member::EVENT_ID].GetInt()));
    }
    // eventIDの指定がなければ、action配列から生成
    else
    {
        return this->factory->createGameEvent(json[member::ACTION]);
    }
}

#pragma mark -
#pragma mark EventSequence

// Sequence
bool EventSequence::init(rapidjson::Value& json)
{
    if (!GameEvent::init()) return false;
    
    if (!this->eventHelper->hasMember(json, member::ACTION)) return false;
    
    _json = json[member::ACTION];
    
    if (!_json.IsArray()) return false;
    
    return true;
}

void EventSequence::run()
{
    // 先頭のイベントを生成して実行
    GameEvent* event { this->factory->createGameEvent(_json[0]) };
    CC_SAFE_RETAIN(event);
    _currentEvent = event;
    
    event->run();
}

void EventSequence::update(float delta)
{
    if (_currentEvent) _currentEvent->update(delta);
    if (_currentEvent && !_currentEvent->isDone()) return;
    
    // 実行中のイベントが終わっていたら解放
    CC_SAFE_RELEASE_NULL(_currentEvent);
    
    // インデックスをあげる
    _currentIdx++;
    
    // インデックス = 要素の個数なら終了
    if (_currentIdx == _json.Size()) {
        this->setDone();
        return;
    }
    
    // 次のイベントを生成
    GameEvent* event { this->factory->createGameEvent(_json[_currentIdx]) };
    CC_SAFE_RETAIN(event);
    _currentEvent = event;
    
    // 次のイベントを実行
    event->run();
}

void EventSequence::stop(int code)
{
    if (!_currentEvent) return;
    
    _currentEvent->stop(code);
}

#pragma mark -
#pragma mark EventSpawn

// Spawn
bool EventSpawn::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    this->events = this->factory->createEventVector(json);
    
    if(this->events.empty()) return false;
    
    return true;
}

void EventSpawn::run()
{
    if(this->events.size() == 0) return;
    
    for(GameEvent* event : this->events)
    {
        event->run();
    }
}

void EventSpawn::update(float delta)
{
    if(this->isDone()) return;
    
    // 持っているイベントを更新し、全て終了していたら自身を終了する
    bool allDone { true };
    
    for (GameEvent* event : this->events)
    {
        event->update(delta);
        
        if(!event->isDone()) allDone = false;
    }
    
    if(allDone)
    {
        this->setDone();
        
        this->events.empty();
    }
}

void EventSpawn::stop(int code)
{
    for (GameEvent* event : this->events)
    {
        event->stop(code);
    }
}

#pragma mark -
#pragma mark EventIf

// If
bool EventIf::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // conditionをチェックしてtrueであればイベントを生成
    if(this->eventHelper->detectCondition(json))
    {
        this->event = this->createSpawnFromIdOrAction(json);
        CC_SAFE_RETAIN(this->event);
        
        return true;
    }
    // falseの場合はその場で終了
    else
    {
        return false;
    }
}

void EventIf::run()
{
    if(!this->event)
    {
        this->setDone();
        return;
    }
    
    this->event->run();
}

void EventIf::update(float delta)
{
    if(!this->event)
    {
        this->setDone();
        
        return;
    }
    
    this->event->update(delta);
    
    if(this->event->isDone())
    {
        this->setDone();
        CC_SAFE_RELEASE_NULL(this->event);
    }
}

#pragma mark -
#pragma mark CallEvent

bool CallEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init()) return false;
    
    EventScript* eventScript  = this->eventHelper->hasMember(json, member::CLASS_NAME) ? DungeonSceneManager::getInstance()->getCommonEventScriptsObject()->getScript(json[member::CLASS_NAME].GetString()) : DungeonSceneManager::getInstance()->getEventScript();
    
    if (!this->eventHelper->hasMember(json, member::EVENT_ID)) return false;
    
    this->event = this->factory->createGameEvent(eventScript->getScriptJson(json[member::EVENT_ID].GetString()));
    CC_SAFE_RETAIN(this->event);
    
    return true;
}

void CallEvent::run(){
    if(!this->event)
    {
        this->setDone();
        return;
    }
    
    this->event->run();
}

void CallEvent::update(float delta)
{
    if (!this->event)
    {
        this->setDone();
        return;
    }
    
    this->event->update(delta);
    
    if (this->event->isDone())
    {
        this->setDone();
        CC_SAFE_RELEASE_NULL(this->event);
    }
}

void CallEvent::stop(int code)
{
    if(this->event != nullptr) this->event->stop(code);
}

#pragma mark -
#pragma mark EventRepeat

// Repeat
bool EventRepeat::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    if(!this->eventHelper->hasMember(json, member::TIMES)) return false;
    
    this->times = json[member::TIMES].GetInt();
    
    if(!this->eventHelper->hasMember(json, member::ACTION)) return false;
    
    if(this->eventHelper->hasMember(json, member::ID)) this->code = stoi(json[member::ID].GetString());
    
    this->event = this->createSpawnFromIdOrAction(json);
    CC_SAFE_RETAIN(this->event);
    this->json = &json;
    
    return true;
}

void EventRepeat::run()
{
    
    if(!this->event || this->times == 0)
    {
        this->setDone();
        CC_SAFE_RELEASE_NULL(this->event);
        return;
    }
    
    this->event->run();
}

void EventRepeat::update(float delta)
{
    
    if (!this->event || this->times == 0)
    {
        this->setDone();
        CC_SAFE_RELEASE_NULL(this->event);
        return;
    }
    
    this->event->update(delta);
    
    if (this->event->isDone())
    {
        this->times--;
        if(this->times == 0)
        {
            this->setDone();
            CC_SAFE_RELEASE_NULL(this->event);
            return;
        }
        // 0でないので再実行
        CC_SAFE_RELEASE_NULL(this->event);
        this->event = this->createSpawnFromIdOrAction(*this->json);
        CC_SAFE_RETAIN(this->event);
        this->event->run();
    }
    
}

void EventRepeat::stop(int code)
{
    if(this->code == code) this->times = 0;
}

#pragma mark -
#pragma mark EventStop

// StopEvent
bool EventStop::init(rapidjson::Value& json)
{
    if (!GameEvent::init()) return false;
    
    if(!this->eventHelper->hasMember(json, member::ID)) return false;
    
    this->eventCode = stoi(json[member::ID].GetString());

    return true;
}

void EventStop::run()
{
    DungeonSceneManager::getInstance()->getRunningEvent()->stop(this->eventCode);
    
    this->setDone();
}

