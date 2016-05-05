//
//  MiniGameEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/06.
//
//

#ifndef MiniGameEvent_h
#define MiniGameEvent_h

#include "Event/GameEvent.h"

#include "Datas/Message/CharacterMessageData.h"

// 連打イベント
class ButtonMashingEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ButtonMashingEvent, rapidjson::Value&)
private:
    int count { 0 };
    float limit { 0.f };
    int sEventId { static_cast<int>(EventID::UNDIFINED) };
    int fEventId { static_cast<int>(EventID::UNDIFINED) };
    GameEvent* sEvent { nullptr };
    GameEvent* fEvent { nullptr };
    GameEvent* event { nullptr };
    GameEvent* clickCallbackEvent { nullptr };
private:
    ButtonMashingEvent() {FUNCLOG};
    ~ButtonMashingEvent();
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// 選択イベント
class SelectEvent : public GameEvent
{
private:
    using SelectCallBack = pair<int, GameEvent*>;
public:
    CREATE_FUNC_WITH_PARAM(SelectEvent, rapidjson::Value&)
private:
    string message {};
    vector<string> choices {};
    vector<SelectCallBack> eventCallBacks {};
    queue<CharacterMessageData*> datas {};
    
private:
    SelectEvent() {FUNCLOG};
    ~SelectEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// パスワードイベント
class PasswordEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(PasswordEvent, rapidjson::Value&)
private:
    string password {};
    int sEventId { static_cast<int>(EventID::UNDIFINED) };
    int fEventId { static_cast<int>(EventID::UNDIFINED) };
    GameEvent* sEvent { nullptr };
    GameEvent* fEvent { nullptr };
    GameEvent* event { nullptr };
private:
    PasswordEvent() {FUNCLOG};
    ~PasswordEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// CountDownイベント
class CountDownEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(CountDownEvent, rapidjson::Value&)
private:
    float second { 5.f };
    int sEventId { static_cast<int>(EventID::UNDIFINED) };
    int fEventId { static_cast<int>(EventID::UNDIFINED) };
    //rapidjson::Document doc { nullptr };
    int equip {0};
    bool checkEquip {false};
    GameEvent* sEvent { nullptr };
    GameEvent* fEvent { nullptr };
    GameEvent* event { nullptr };
private:
    CountDownEvent() {FUNCLOG};
    ~CountDownEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// StopCountイベント
class StopCountEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(StopCountEvent, rapidjson::Value&);
private:
    StopCountEvent() {FUNCLOG};
    ~StopCountEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

#endif /* MiniGameEvent_h */
