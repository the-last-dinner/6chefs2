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
    string layerName {};
private:
    HideLayerEvent() {FUNCLOG};
    ~HideLayerEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// マップの指定レイヤを揺らす
class SwingLayerEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(SwingLayerEvent, rapidjson::Value&)
private:
    string layerName {};
private:
    SwingLayerEvent() {FUNCLOG};
    ~SwingLayerEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// マップのレイヤのアクションを止める
class StopLayerActionEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(StopLayerActionEvent, rapidjson::Value&)
private:
    StopLayerActionEvent() {FUNCLOG};
    ~StopLayerActionEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

#endif /* defined(__LastSupper__MapEvent__) */
