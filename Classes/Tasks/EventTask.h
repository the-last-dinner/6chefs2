//
//  EventTask.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/05.
//
//

#ifndef __LastSupper__EventScriptTask__
#define __LastSupper__EventScriptTask__

#include "Tasks/GameTask.h"

class GameEvent;
class EventScript;
class EventQueue;

class EventTask : public GameTask
{
// 定数
private:
    using EventWithId = pair<int, GameEvent*>;
    using CallbackWithId = pair<int, function<void()>>;
// クラスメソッド
public:
    CREATE_FUNC(EventTask)
    CREATE_FUNC_WITH_PARAM(EventTask, EventScript*)

// インスタンス変数
public:
    function<void()> _onEventStart { nullptr };
    function<void()> _onEventFinished { nullptr };
private:
    EventScript* _eventScript { nullptr };
    deque<EventWithId> _eventQueue {};
    vector<GameEvent*> _asyncEvents {};
    EventWithId _runningEvent {EventWithId({static_cast<int>(EventID::UNDIFINED), nullptr})};
    CallbackWithId _callbackInfo {CallbackWithId({static_cast<int>(EventID::UNDIFINED), nullptr})};
    
// インスタンスメソッド
public:
    // 実行
    void runEventQueue();
	void runEvent(int eventId);
    void runEvent(vector<int> eventIds, function<void()> callback = nullptr);
    void runEvent(GameEvent* event, function<void()> callback = nullptr);
    void runEventAsync(GameEvent* event);
    
    // キュー操作
    bool pushEventBack(int eventId);
    bool pushEventFront(int eventId);
    void pushEventBack(GameEvent* event);
    void pushEventFront(GameEvent* event);

    bool isEventRunning();
    bool existsEvent();
    int getRunningEventId() const;
    GameEvent* getRunningEvent() const;
    deque<EventWithId> getEvents() const;
    
    EventScript* getEventScript() const;
    
    void update(float delta);
    void updateForAsync(float delta);
    
    void releaseEventIfNeeded(GameEvent* event);
private:
    EventTask();
    ~EventTask();
    bool init();
    bool init(EventScript* eventScript);
    
    void run();
    GameEvent* createEventById(int eventId);
    int getEventId(const EventWithId& eventWithId) const;
    GameEvent* getGameEvent(const EventWithId& eventWithId) const;
};

#endif /* defined(__LastSupper__EventScriptTask__) */
