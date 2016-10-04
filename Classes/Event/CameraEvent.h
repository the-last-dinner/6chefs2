//
//  CameraEvent.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/10/03.
//
//

#ifndef CameraEvent_h
#define CameraEvent_h

#include "Event/GameEvent.h"

// カメラ生成イベント
class CreateCameraEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(CreateCameraEvent, rapidjson::Value&)
private:
    Location _location {};
    int _objId { etoi(ObjectID::UNDIFINED) };
    rapidjson::Value _eventJson {};
private:
    CreateCameraEvent() { FUNCLOG };
    ~CreateCameraEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// カメラ移動イベント
class MoveCameraEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(MoveCameraEvent, rapidjson::Value&)
private:
    Point _toPosition { Point::ZERO };
    float _duration { 0.f };
private:
    MoveCameraEvent() { FUNCLOG };
    ~MoveCameraEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// カメラターゲット変更イベント
class SetCameraTargetEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(SetCameraTargetEvent, rapidjson::Value&)
private:
    int _objectId { etoi(ObjectID::UNDIFINED) };
private:
    SetCameraTargetEvent() { FUNCLOG };
    ~SetCameraTargetEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

#endif /* CameraEvent_h */
