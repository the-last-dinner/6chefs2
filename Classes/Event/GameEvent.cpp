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
    CC_SAFE_RELEASE_NULL(_factory);
    CC_SAFE_RELEASE_NULL(_eventHelper);
};

// 初期化
bool GameEvent::init(rapidjson::Value& json)
{
    EventFactory* factory {DungeonSceneManager::getInstance()->getEventFactory()};
    GameEventHelper* eventHelper {DungeonSceneManager::getInstance()->getGameEventHelper()};
    
    if(!factory || !eventHelper) return false;
    
    CC_SAFE_RETAIN(factory);
    _factory = factory;
    
    CC_SAFE_RETAIN(eventHelper);
    _eventHelper = eventHelper;
    
    _json.CopyFrom(json, DungeonSceneManager::getInstance()->getEventScript()->getDocument().GetAllocator());
    
    return true;
}

// 呼び出し元イベントを設定
void GameEvent::setCaller(const GameEvent* caller)
{
    _caller = caller;
}

// イベントIDを設定
void GameEvent::setEventId(int eventId)
{
    _id = eventId;
}

// イベントIDを取得
int GameEvent::getEventId() const
{
    if (_caller) return _caller->getEventId();
    
    return _id;
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
    if (_eventHelper->hasMember(json, member::EVENT_ID)) {
        rapidjson::Value eventJson { DungeonSceneManager::getInstance()->getEventScript()->getScriptJson(stoi(json[member::EVENT_ID].GetString())) };
        GameEvent* event { _factory->createGameEvent(eventJson, nullptr) };
        event->setEventId(stoi(json[member::EVENT_ID].GetString()));
        
        return event;
    }
    // eventIDの指定がなければ、action配列から生成
    else {
        return _factory->createGameEvent(json[member::ACTION], this);
    }
}

#pragma mark -
#pragma mark EventSequence

// Sequence
bool EventSequence::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::ACTION)) return false;
    
    _json = _json[member::ACTION];
    
    if (!_json.IsArray()) return false;
    
    return true;
}

void EventSequence::run()
{
    // 先頭のイベントを生成して実行
    GameEvent* event { _factory->createGameEvent(_json[0], this) };
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
    GameEvent* event { _factory->createGameEvent(_json[_currentIdx], this) };
    CC_SAFE_RETAIN(event);
    _currentEvent = event;
    
    if (!event) {
        this->setDone();
        return;
    }
    
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
    if (!GameEvent::init(json)) return false;
    
    rapidjson::Value& eventJson { (_json.IsObject() && _json.HasMember(member::ACTION)) ? _json[member::ACTION] : _json };
    
    for(int i { 0 }; i < eventJson.Size(); i++) {
        if(GameEvent* event { _factory->createGameEvent(eventJson[i], this) }) {
            _events.pushBack(event);
        }
    }
    
    if (_events.empty()) return false;
    
    return true;
}

void EventSpawn::run()
{
    if (_events.size() == 0) return;
    
    for (GameEvent* event : _events) {
        event->run();
    }
}

void EventSpawn::update(float delta)
{
    if (this->isDone()) return;
    
    // 持っているイベントを更新し、全て終了していたら自身を終了する
    bool allDone { true };
    
    for (GameEvent* event : _events) {
        if(event->isDone()) continue;
        
        event->update(delta);
        
        if (!event->isDone()) allDone = false;
    }
    
    if (allDone) {
        this->setDone();
        
        _events.empty();
    }
}

void EventSpawn::stop(int code)
{
    for (GameEvent* event : _events) {
        event->stop(code);
    }
}

#pragma mark -
#pragma mark EventIf

// If
bool EventIf::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    return true;
}

void EventIf::run()
{
    // conditionをチェックしてtrueであればイベントを生成
    if (_eventHelper->detectCondition(_json)) {
        _event = this->createSpawnFromIdOrAction(_json);
        CC_SAFE_RETAIN(_event);
    }
    
    if (_event) _event->run();
}

void EventIf::update(float delta)
{
    if (!_event) {
        this->setDone();
        return;
    }
    
    _event->update(delta);
    
    if (_event->isDone()) {
        this->setDone();
        CC_SAFE_RELEASE_NULL(_event);
    }
}

#pragma mark -
#pragma mark CallEvent

bool CallEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    if (!_eventHelper->hasMember(_json, member::EVENT_ID)) return false;
    
    /** @FIXME 本当はrunで格納したいけど、現状マップ移動するとjsonを維持できないからここで入れてる **/
    _eventId = _json[member::EVENT_ID].GetString();
    
    _className = _eventHelper->hasMember(_json, member::CLASS_NAME) ? _json[member::CLASS_NAME].GetString() : "";
    
    return true;
}

void CallEvent::run()
{
    EventScript* eventScript { nullptr };
    
    if (_className != "") {
        eventScript = DungeonSceneManager::getInstance()->getCommonEventScriptsObject()->getScript(_className);
    } else {
        eventScript = DungeonSceneManager::getInstance()->getEventScript();
    }
    
    rapidjson::Value eventJson { eventScript->getScriptJson(_eventId) };
    rapidjson::Value nullValue;
    if (eventJson == nullValue) return;
    
    _event = _factory->createGameEvent(eventJson, nullptr);
    CC_SAFE_RETAIN(_event);
    
    if (_event) {
        /** FIXME eventIDはstringで入れたいけど、今はintしか無理だから、現状callEventは-2で格納 **/
        int eventIdInt = -2;
        if (LastSupper::StringUtils::isNumericString(_eventId)) {
            eventIdInt = stoi(_eventId);
        }
        _event->setEventId(eventIdInt);
        _event->run();
    }
}

void CallEvent::update(float delta)
{
    if (!_event) {
        this->setDone();
        return;
    }
    
    _event->update(delta);
    
    if (_event->isDone()) {
        this->setDone();
        CC_SAFE_RELEASE_NULL(_event);
    }
}

void CallEvent::stop(int code)
{
    if (_event != nullptr) _event->stop(code);
}

#pragma mark -
#pragma mark EventRepeat

// Repeat
bool EventRepeat::init(rapidjson::Value& json)
{
 
    if (!GameEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::TIMES)) return false;
    
    _times = _json[member::TIMES].GetInt();
    
    if (!_eventHelper->hasMember(_json, member::ACTION)) return false;
    
    if (_eventHelper->hasMember(_json, member::ID)) _code = stoi(_json[member::ID].GetString());
    
    _event = this->createSpawnFromIdOrAction(_json);
    
    CC_SAFE_RETAIN(_event);
    
    return true;
}

void EventRepeat::run()
{
    
    if (!_event || _times == 0) {
        this->setDone();
        CC_SAFE_RELEASE_NULL(_event);
        return;
    }
    
    _event->run();
}

void EventRepeat::update(float delta)
{
    
    if (!_event || _times == 0) {
        this->setDone();
        CC_SAFE_RELEASE_NULL(_event);
        return;
    }
    
    _event->update(delta);
    
    if (_event->isDone()) {
        _times--;
        if (_times == 0) {
            this->setDone();
            CC_SAFE_RELEASE_NULL(_event);
            return;
        }
        // 0でないので再実行
        CC_SAFE_RELEASE_NULL(_event);
        _event = this->createSpawnFromIdOrAction(_json);
        CC_SAFE_RETAIN(_event);
        _event->run();
    }
    
}

void EventRepeat::stop(int code)
{
    if (_code == code) _times = 0;
}

#pragma mark -
#pragma mark EventStop

// StopEvent
bool EventStop::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::ID)) return false;
    
    _eventCode = stoi(_json[member::ID].GetString());

    return true;
}

void EventStop::run()
{
    DungeonSceneManager::getInstance()->getRunningEvent()->stop(_eventCode);
    
    this->setDone();
}

