//
//  EventTask.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/05.
//
//

#include "Tasks/EventTask.h"

#include "Event/GameEvent.h"
#include "Event/EventFactory.h"

#include "Event/EventScript.h"
#include "Managers/DungeonSceneManager.h"

// コンストラクタ
EventTask::EventTask(){FUNCLOG}

// デストラクタ
EventTask::~EventTask()
{
    FUNCLOG
    
    // イベントキューが空でなかったら全て解放
    if(!_eventQueue.empty())
    {
        for(int i {0}; i < _eventQueue.size(); i++)
        {
            CC_SAFE_RELEASE(this->getGameEvent(_eventQueue.front()));
            _eventQueue.pop_front();
        }
    }
    
    // 実行中のイベントを解放
    CC_SAFE_RELEASE_NULL(_runningEvent.second);
    
    // スクリプトを解放
    CC_SAFE_RELEASE_NULL(_eventScript);
}

// 初期化
bool EventTask::init()
{
    EventScript * eventScript { EventScript::create(CsvDataManager::getInstance()->getMapData()->getFileName(DungeonSceneManager::getInstance()->getLocation().map_id)) };
    return this->init(eventScript);
}

bool EventTask::init(EventScript* eventScript)
{
    if(!GameTask::init()) return false;
    
    // イベントスクリプト生成
    _eventScript = eventScript;
    CC_SAFE_RETAIN(_eventScript);
    
    // update開始
    this->scheduleUpdate();
    
    return true;
}

// イベントキューにあるイベントを実行開始
void EventTask::runEventQueue()
{
    if(!this->existsEvent()) return;
    this->run();
}

// イベントをIDから実行
void EventTask::runEvent(int eventId)
{
    if(eventId == etoi(EventID::UNDIFINED)) return;
    this->pushEventBack(eventId);
    
    // 実行
    this->runEventQueue();
}

// イベントをIDベクタから実行、ベクタを全て実行するとコールバック呼び出し
void EventTask::runEvent(vector<int> eventIds, function<void()> callback)
{
    // ベクタが空ならコールバック
    if(eventIds.empty())
    {
        if(callback) callback();
        
        return;
    }
    
    int pushedNum { 0 };
    int lastEventId { 0 };
    
    for(int eventId : eventIds)
    {
        if(this->pushEventBack(eventId))
        {
            pushedNum++;
            lastEventId = eventId;
        }
    }
    
    // 一つも生成されていなければその場でコールバック
    if(pushedNum == 0)
    {
        if(callback) callback();
        
        return;
    }
    
    // キューにあるイベントを先頭から実行
    this->runEventQueue();
    
    _callbackInfo = CallbackWithId({lastEventId, callback});
}

// イベントを実行
void EventTask::runEvent(GameEvent* event, function<void()> callback)
{
    this->pushEventBack(event);
    
    _callbackInfo = CallbackWithId({static_cast<int>(EventID::UNDIFINED), callback});
    
    this->runEventQueue();
}

// 非同期イベント実行
void EventTask::runEventAsync(GameEvent *event)
{
    if(!event) return;
    
    event->run();
    _asyncEvents.push_back(event);
}

// キューに指定IDイベントを後ろから詰める
bool EventTask::pushEventBack(int eventId)
{
    GameEvent* event {this->createEventById(eventId)};
    if(!event) return false;
    _eventQueue.push_back({eventId, event});
    
    return true;
}

// キューに指定IDイベントを前から詰める
bool EventTask::pushEventFront(int eventId)
{
    GameEvent* event {this->createEventById(eventId)};
    if(!event) return false;
   _eventQueue.push_front({eventId, event});
    
    return true;
}

// キューにイベントを後ろから詰める、EventIDはUNDIFINEDを使用
void EventTask::pushEventBack(GameEvent* event)
{
    if(!event) return;
    
    _eventQueue.push_back({etoi(EventID::UNDIFINED), event});
}

// キューにイベントを前から詰める、EventIDは現在実行中のものを使用
void EventTask::pushEventFront(GameEvent* event)
{
    if(!event) return;
    
    _eventQueue.push_front({this->getEventId(_runningEvent), event});
}

// 現在実行中のイベントがあるか
bool EventTask::isEventRunning()
{
    return this->getGameEvent(_runningEvent);
}

// キューにイベントが存在するか
bool EventTask::existsEvent()
{
    return !_eventQueue.empty();
}

// EventScriptを取得
EventScript* EventTask::getEventScript() const
{
    return _eventScript;
}

// update
void EventTask::update(float delta)
{
    this->updateForAsync(delta);
    
    // 実行中イベントがnullptrなら無視
    if(!this->getGameEvent(_runningEvent)) return;
    
    // 実行中イベントを更新
    this->getGameEvent(_runningEvent)->update(delta);
    
    // 実行中イベントが終了していたら解放
    if(this->getGameEvent(_runningEvent)->isDone())
    {
        if(_callbackInfo.second && _callbackInfo.first == this->getEventId(_runningEvent))
        {
            function<void()> cb { _callbackInfo.second };
            _callbackInfo = CallbackWithId({static_cast<int>(EventID::UNDIFINED), nullptr});
            cb();
        }
        this->releaseEventIfNeeded(this->getGameEvent(_runningEvent)) ;
        _runningEvent = EventWithId({static_cast<int>(EventID::UNDIFINED), nullptr});
    }
        
    // キューが空になったらコールバック
    if(_eventQueue.empty() && !this->getGameEvent(_runningEvent))
    {
        if(_onEventFinished) _onEventFinished();
    }
    
    // イベントを実行
    this->run();
}

// 非同期イベント用update
void EventTask::updateForAsync(float delta)
{
    vector<GameEvent*>::iterator itr = _asyncEvents.begin();
    while(itr != _asyncEvents.end())
    {
        // イベントを更新
        (*itr)->update(delta);
        
        // イベントが終了していたら削除
        if((*itr)->isDone())
        {
            this->releaseEventIfNeeded(*itr);
            itr = _asyncEvents.erase(itr);
        }
        else
        {
            itr++;
        }
    }
}

// 必要ならばイベントインスタンスを解放
void EventTask::releaseEventIfNeeded(GameEvent *event)
{
    if(event->isReusable())
    {
        event->setDone(false);
    }
    else
    {
        CC_SAFE_RELEASE_NULL(event);
    }
}

// 実行中のイベントIDを取得
int EventTask::getRunningEventId() const
{
    return this->getEventId(_runningEvent);
}

// 実行中のイベントを取得
GameEvent* EventTask::getRunningEvent() const
{
    return this->getGameEvent(_runningEvent);
}

// キューにあるイベントを全て取得
deque<EventTask::EventWithId> EventTask::getEvents() const
{
    return _eventQueue;
}

#pragma mark -
#pragma mark private

// キューにある先頭のイベントを実行
void EventTask::run()
{
    // 実行中のイベントがあれば無視
    if(this->isEventRunning()) return;
    
    // イベントキューにイベントがなければ無視
    if(!this->existsEvent()) return;
    
    // なければ先頭を実行
    _runningEvent = _eventQueue.front();
    _eventQueue.pop_front();
    this->getGameEvent(_runningEvent)->run();
    
    // イベント開始をコールバック
    if(_onEventStart) _onEventStart();
}

// IDからイベントを生成
GameEvent* EventTask::createEventById(int eventId)
{
    if(eventId == static_cast<int>(EventID::UNDIFINED)) return nullptr;
    if(PlayerDataManager::getInstance()->getLocalData()->checkEventIsDone(DungeonSceneManager::getInstance()->getLocation().map_id, eventId)) return nullptr;
    
    DungeonSceneManager* manager {DungeonSceneManager::getInstance()};
    
    rapidjson::Value json { _eventScript->getScriptJson(eventId) };
    GameEvent* event { manager->getEventFactory()->createGameEvent(json, nullptr) };
    CC_SAFE_RETAIN(event);
    
    if (event) event->setEventId(eventId);
    
    return event;
}

// イベントIDを取得
int EventTask::getEventId(const EventWithId& eventWithId) const
{
    return eventWithId.first;
}

// イベントポインタを取得
GameEvent* EventTask::getGameEvent(const EventWithId& eventWithId) const
{
    return eventWithId.second;
}
