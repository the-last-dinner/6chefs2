//
//  MapEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/27.
//
//

#ifndef __LastSupper__MapEvent__
#define __LastSupper__MapEvent__

#include "Event/GameEvent.h"

// マップに関するイベント

// マップの指定レイヤを非表示にする
class HideLayerEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(HideLayerEvent, rapidjson::Value&)
private:
    string _layerName {};
private:
    HideLayerEvent() { FUNCLOG };
    ~HideLayerEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

class ShowLayerEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ShowLayerEvent, rapidjson::Value&)
private:
    string _layerName {};
private:
    ShowLayerEvent() { FUNCLOG };
    ~ShowLayerEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// マップの指定レイヤを揺らす
class SwingLayerEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(SwingLayerEvent, rapidjson::Value&)
private:
    string _layerName {};
private:
    SwingLayerEvent() { FUNCLOG };
    ~SwingLayerEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// マップのレイヤのアクションを止める
class StopLayerActionEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(StopLayerActionEvent, rapidjson::Value&)
private:
    StopLayerActionEvent() { FUNCLOG };
    ~StopLayerActionEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// マップレイヤを揺らす
class QuakeMapEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(QuakeMapEvent, rapidjson::Value&)
private:
    int _x { 0 };
    int _y { 0 };
    float _time { 0.0f };
private:
    QuakeMapEvent() { FUNCLOG };
    ~QuakeMapEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

#endif /* defined(__LastSupper__MapEvent__) */
