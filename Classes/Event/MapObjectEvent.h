//
//  MapObjectEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#ifndef __LastSupper__MapObjectEvent__
#define __LastSupper__MapObjectEvent__

#include "Event/GameEvent.h"

class MapObject;
class Light;

// マップオブジェクトに関わるイベント
// 基底クラス
class MapObjectEvent : public GameEvent
{
// インスタンス変数
protected:
    string objectId {};
    
// インスタンスメソッド
protected:
    MapObjectEvent() {};
    ~MapObjectEvent() {};
    virtual bool init(rapidjson::Value& json);
};

// リアクション
class ReactionEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ReactionEvent, rapidjson::Value&)
private:
    ReactionEvent() {FUNCLOG};
    ~ReactionEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// マップオブジェクトを生成
class CreateMapObjectEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(CreateMapObjectEvent, rapidjson::Value&)
private:
    CreateMapObjectEvent() {FUNCLOG};
    ~CreateMapObjectEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

//  マップオブジェクトを削除
class RemoveMapObjectEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(RemoveMapObjectEvent, rapidjson::Value&)
private:
    RemoveMapObjectEvent() {FUNCLOG};
    ~RemoveMapObjectEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// パーティを追加
class FollowCharacterEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(FollowCharacterEvent, rapidjson::Value&)
private:
    FollowCharacterEvent() {FUNCLOG};
    ~FollowCharacterEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// パーティを解除
class ReleaseFollowingCharacterEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ReleaseFollowingCharacterEvent, rapidjson::Value&)
private:
    ReleaseFollowingCharacterEvent() {FUNCLOG};
    ~ReleaseFollowingCharacterEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// マップオブジェクトを再配置
class WarpMapObjectEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(WarpMapObjectEvent, rapidjson::Value&)
private:
    Point point {Point(0,0)};
    Direction direction {Direction::FRONT};
private:
    WarpMapObjectEvent(){FUNCLOG};
    ~WarpMapObjectEvent(){FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// マップオブジェクトを目的地に動かす
class MoveToEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(MoveToEvent, rapidjson::Value&)
private:
    Point dest {Point::ZERO};
    float speedRatio { 1.0f };
private:
    MoveToEvent() {FUNCLOG};
    ~MoveToEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// マップオブジェクトを、向きと格子数を指定して移動
class MoveByEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(MoveByEvent, rapidjson::Value&)
private:
    Direction direction {Direction::SIZE};
    int gridNum {0};
    float speedRatio {1.f};
private:
    MoveByEvent() {FUNCLOG};
    ~MoveByEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// 光をセット
class SetLightEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(SetLightEvent, rapidjson::Value&)
private:
    Light* light { nullptr };
private:
    SetLightEvent() {FUNCLOG};
    ~SetLightEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// 光を消す
class RemoveLightEvent : public MapObjectEvent
{
public:
    CREATE_FUNC_WITH_PARAM(RemoveLightEvent, rapidjson::Value&)
private:
    RemoveLightEvent() {FUNCLOG};
    ~RemoveLightEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

#endif /* defined(__LastSupper__MapObjectEvent__) */
