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

class EventTask : public GameTask
{
// 定数
private:
    using EventWithId = pair<int, GameEvent*>;
    using CallbackWithId = pair<int, function<void()>>;
// クラスメソッド
public:
    CREATE_FUNC(EventTask)

// インスタンス変数
public:
    function<void()> onRunEvent { nullptr };
    function<void()> onAllEventFinished { nullptr };
private:
    EventScript* eventScript { nullptr };
    deque<EventWithId> eventQueue {};
    EventWithId runningEvent {EventWithId({static_cast<int>(EventID::UNDIFINED), nullptr})};
    CallbackWithId callbackInfo {CallbackWithId({static_cast<int>(EventID::UNDIFINED), nullptr})};
    int pushingEventId {etoi(EventID::UNDIFINED)};
    
// インスタンスメソッド
public:
    // 実行
    void runEventQueue();
	void runEvent(int eventId);
    void runEvent(vector<int> eventIds, function<void()> callback = nullptr);
    void runEvent(GameEvent* event, function<void()> callback = nullptr);
    
    // キュー操作
    bool pushEventBack(int eventId);
    bool pushEventFront(int eventId);
    void pushEventBack(GameEvent* event);
    void pushEventFront(GameEvent* event);

    bool isEventRunning();
    bool existsEvent();
    int getRunningEventId() const;
    deque<EventWithId> getEvents() const;
    
    // キュー中のevent_id操作
    int getPushingEventId() const;
    void setPushingEventId(const int event_id);
    void resetPushingEventId();
    
    EventScript* getEventScript() const;
    
    void update(float delta);
private:
    EventTask();
    ~EventTask();
    bool init();
    
    void run();
    GameEvent* createEventById(int eventId);
    int getEventId(const EventWithId& eventWithId) const;
    GameEvent* getGameEvent(const EventWithId& eventWithId) const;
};

#endif /* defined(__LastSupper__EventScriptTask__) */
