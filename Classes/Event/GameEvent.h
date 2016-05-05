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
class EventScriptValidator;

// ゲーム上のイベント基底クラス
class GameEvent : public Ref
{
// インスタンス変数
protected:
    EventFactory* factory { nullptr };
    EventScriptValidator* validator { nullptr };
private:
    bool _isDone {false};
    bool _isReusable {false};
    
// インスタンスメソッド
public:
    bool isReusable() const;
    void setReusable(bool reusable);
    bool isDone() const;
    void setDone(bool done=true);
    virtual void run() {CCLOG("runメソッドをoverrideしてね");};     // イベント開始
    virtual void update(float delta) {};                         // タスクによって毎フレーム呼び出されるメソッド
protected:
    GameEvent();
    virtual ~GameEvent();
    virtual bool init();
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
    queue<GameEvent*> events {};
    
// インスタンスメソッド
private:
    EventSequence() {FUNCLOG};
    ~EventSequence() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
    virtual void update(float delta) override;
};

// Spawn
class EventSpawn : public GameEvent
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EventSpawn, rapidjson::Value&)
    
// インスタンス変数
private:
    Vector<GameEvent*> events {};
    
// インスタンスメソッド
private:
    EventSpawn() {FUNCLOG};
    ~EventSpawn() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
    virtual void update(float delta) override;
};

// If
class EventIf : public GameEvent
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EventIf, rapidjson::Value&)

// インスタンス変数
private:
    GameEvent* event { nullptr };

// インスタンスメソッド
private:
    EventIf() {FUNCLOG};
    ~EventIf() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
    virtual void update(float delta) override;
};

// CallEvent
class CallEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(CallEvent, rapidjson::Value&)
private:
    GameEvent* event {nullptr};
private:
    CallEvent() {FUNCLOG};
    ~CallEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
    virtual void update(float delta) override;
};

// Repeat
class EventRepeat : public GameEvent
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EventRepeat, rapidjson::Value&)
    
// インスタンス変数
private:
    int times { 0 };
    GameEvent* event {nullptr};
    rapidjson::Value json {};
    
// インスタンスメソッド
private:
    EventRepeat() {FUNCLOG};
    ~EventRepeat() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
    virtual void update(float delta) override;
};

#endif /* defined(__LastSupper__GameEvent__) */
