//
//  GameEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/23.
//
//

#ifndef __LastSupper__GameEvent__
#define __LastSupper__GameEvent__

#include "Common.h"

class EventFactory;
class GameEventHelper;

// ゲーム上のイベント基底クラス
class GameEvent : public Ref
{
// インスタンス変数
protected:
    EventFactory* _factory { nullptr };
    GameEventHelper* _eventHelper { nullptr };
    rapidjson::Value _json {};
    const GameEvent* _caller { nullptr };
    int _id { etoi(EventID::UNDIFINED) };
    int _code { -1 };
private:
    bool _isDone { false };
    bool _isReusable { false };
    
// インスタンスメソッド
public:
    void setCaller(const GameEvent* parent);
    void setEventId(int eventId);
    int getEventId() const;
    bool isReusable() const;
    void setReusable(bool reusable);
    bool isDone() const;
    void setDone(bool done = true);
    
// インターフェース
public:
    virtual void run() { CCLOG("runメソッドをoverrideしてね"); };     // イベント開始
    virtual void update(float delta) {};                           // タスクによって毎フレーム呼び出されるメソッド
    virtual void stop(int code = -1) {};
    
protected:
    GameEvent();
    virtual ~GameEvent();
    virtual bool init(rapidjson::Value& json);
    GameEvent* createSpawnFromIdOrAction(rapidjson::Value& json);   // イベントIDもしくはaction配列からspawnを生成
};

// Sequence
class EventSequence : public GameEvent
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EventSequence, rapidjson::Value&)
    
// インスタンス変数
private:
    int _currentIdx { 0 };
    GameEvent* _currentEvent { nullptr };
    
// インスタンスメソッド
private:
    EventSequence() { FUNCLOG };
    ~EventSequence() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    virtual void update(float delta) override;
    virtual void stop(int code = -1) override;
};

// Spawn
class EventSpawn : public GameEvent
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EventSpawn, rapidjson::Value&)
    
// インスタンス変数
private:
    Vector<GameEvent*> _events {};
    
// インスタンスメソッド
private:
    EventSpawn() {FUNCLOG};
    ~EventSpawn() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    virtual void update(float delta) override;
    virtual void stop(int code = -1) override;

};

// If
class EventIf : public GameEvent
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EventIf, rapidjson::Value&)

// インスタンス変数
private:
    GameEvent* _event { nullptr };

// インスタンスメソッド
private:
    EventIf() {FUNCLOG};
    ~EventIf() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    virtual void update(float delta) override;
};

// CallEvent
class CallEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(CallEvent, rapidjson::Value&)
private:
    GameEvent* _event { nullptr };
    string _eventId;
    string _className;
private:
    CallEvent() {FUNCLOG};
    ~CallEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    virtual void update(float delta) override;
    virtual void stop(int code = -1) override;
};

// Repeat
class EventRepeat : public GameEvent
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EventRepeat, rapidjson::Value&)
    
// インスタンス変数
private:
    int _times { 0 };
    GameEvent* _event { nullptr };
    
// インスタンスメソッド
private:
    EventRepeat() {FUNCLOG};
    ~EventRepeat() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    virtual void update(float delta) override;
    virtual void stop(int code = -1) override;
};

// StopEvent
class EventStop : public GameEvent
{
    // クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EventStop, rapidjson::Value&)
    
    // インスタンス変数
private:
    int _eventCode { -1 };
    
    // インスタンスメソッド
private:
    EventStop() {FUNCLOG};
    ~EventStop() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

#endif /* defined(__LastSupper__GameEvent__) */
