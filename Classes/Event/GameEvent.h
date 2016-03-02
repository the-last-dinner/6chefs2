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
    
// インスタンスメソッド
public:
    bool isDone() const;
    virtual void run() {CCLOG("runメソッドをoverrideしてね");};     // イベント開始
    virtual void update(float delta) {};                         // タスクによって毎フレーム呼び出されるメソッド
protected:
    GameEvent();
    ~GameEvent();
    virtual bool init();
    void setDone();
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
    virtual void update(float delta);
};

#endif /* defined(__LastSupper__GameEvent__) */
