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
    int _count { 0 };
    float _limit { 0.f };
    GameEvent* _resultCallbackEvent { nullptr };
    GameEvent* _clickCallbackEvent { nullptr };
private:
    ButtonMashingEvent() { FUNCLOG };
    ~ButtonMashingEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    virtual void update(float delta) override;
};

// 選択イベント
class SelectEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(SelectEvent, rapidjson::Value&)
private:
    string _message {};
    vector<string> _choices {};
    queue<CharacterMessageData*> _datas {};
    GameEvent* _event { nullptr };
    
private:
    SelectEvent() { FUNCLOG };
    ~SelectEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    virtual void update(float delta) override;
    virtual void stop(int code = -1) override;
};

// パスワードイベント
class PasswordEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(PasswordEvent, rapidjson::Value&)
private:
    string _password {};
    GameEvent* _resultCallbackEvent { nullptr };
private:
    PasswordEvent() {FUNCLOG};
    ~PasswordEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    virtual void update(float delta) override;
};

// CountDownイベント
class CountDownEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(CountDownEvent, rapidjson::Value&)
private:
    float _second { 5.f };
    int _equip { 0 };
    bool _checkEquip { false };
    bool _display { false };
    bool _isTimeUp { false };
    GameEvent* _successCallbackEvent { nullptr };
    GameEvent* _failureCallbackEvent { nullptr };
private:
    CountDownEvent() { FUNCLOG };
    ~CountDownEvent();
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    void runResultCallbackEvent(GameEvent* callbackEvent);
};

// StopCountイベント
class StopCountEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(StopCountEvent, rapidjson::Value&);
private:
    StopCountEvent() {FUNCLOG};
    ~StopCountEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

#endif /* MiniGameEvent_h */
