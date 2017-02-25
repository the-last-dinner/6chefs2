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
    string _objectId {};
    
// インスタンスメソッド
protected:
    CharacterEvent() {};
    virtual ~CharacterEvent() {};
    virtual bool init(rapidjson::Value& json) override;
    Character* getTargetByObjectId(const string& objectId);
};

// 方向転換
class ChangeDirectionEvent : public CharacterEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeDirectionEvent, rapidjson::Value&)
private:
    Direction _direction { Direction::NONE };
private:
    ChangeDirectionEvent() { FUNCLOG };
    ~ChangeDirectionEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// 方向、歩数を指定して歩行
class WalkByEvent : public CharacterEvent
{
public:
    CREATE_FUNC_WITH_PARAM(WalkByEvent, rapidjson::Value&)
private:
    Direction _direction { Direction::NONE };
    int _gridNum { 0 };
    float _speedRatio { 1.f };
    bool _back { false };
private:
    WalkByEvent() { FUNCLOG };
    ~WalkByEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// 移動先座標指定歩行
class WalkToEvent : public CharacterEvent
{
public:
    CREATE_FUNC_WITH_PARAM(WalkToEvent, rapidjson::Value&)
private:
    Point _destPosition { Point::ZERO };
    float _speedRatio { 1.f };
private:
    WalkToEvent() { FUNCLOG };
    ~WalkToEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// 主人公を変更
class ChangeHeroEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeHeroEvent, rapidjson::Value&)
private:
    int _charaId { 0 };
private:
    ChangeHeroEvent() { FUNCLOG };
    ~ChangeHeroEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// キャラクターのスピードを変更
class ChangeSpeedEvent : public CharacterEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeSpeedEvent, rapidjson::Value&);
private:
    float _speed { 1.f };
private:
    ChangeSpeedEvent() { FUNCLOG }
    ~ChangeSpeedEvent() { FUNCLOG }
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

#endif /* defined(__LastSupper__CharacterEvent__) */
