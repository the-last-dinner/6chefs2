//
//  CharacterEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/27.
//
//

#ifndef __LastSupper__CharacterEvent__
#define __LastSupper__CharacterEvent__

#include "Event/GameEvent.h"

class Character;

// キャラクタ系イベント基底クラス
class CharacterEvent : public GameEvent
{
// インスタンス変数
protected:
    string objectId {};
    Character* target { nullptr };
    
// インスタンスメソッド
protected:
    CharacterEvent() {};
    ~CharacterEvent() {};
    virtual bool init(rapidjson::Value& json);
    bool onRun();
};

// 方向転換
class ChangeDirectionEvent : public CharacterEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeDirectionEvent, rapidjson::Value&)
private:
    Direction direction {Direction::SIZE};
private:
    ChangeDirectionEvent() {FUNCLOG};
    ~ChangeDirectionEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// 方向、歩数を指定して歩行
class WalkByEvent : public CharacterEvent
{
public:
    CREATE_FUNC_WITH_PARAM(WalkByEvent, rapidjson::Value&)
private:
    Direction direction {Direction::SIZE};
    int gridNum {0};
    float speedRatio {1.f};
    bool back { false };
private:
    WalkByEvent() {FUNCLOG};
    ~WalkByEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// 移動先座標指定歩行
class WalkToEvent : public CharacterEvent
{
public:
    CREATE_FUNC_WITH_PARAM(WalkToEvent, rapidjson::Value&)
private:
    Point destPosition { Point::ZERO };
    float speedRatio {1.f};
private:
    WalkToEvent() {FUNCLOG};
    ~WalkToEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

#endif /* defined(__LastSupper__CharacterEvent__) */
