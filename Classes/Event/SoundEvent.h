//
//  SoundEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#ifndef __LastSupper__SoundEvent__
#define __LastSupper__SoundEvent__

#include "Event/GameEvent.h"

// BGM再生イベント
class PlayBGMEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(PlayBGMEvent, rapidjson::Value&)
private:
    string fileName {};
    float volume { 1.f };
private:
    PlayBGMEvent():fileName("") {FUNCLOG};
    ~PlayBGMEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// BGM停止イベント
class StopBGMEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(StopBGMEvent, rapidjson::Value&)
private:
    string fileName {};
private:
    StopBGMEvent():fileName("") {FUNCLOG};
    ~StopBGMEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// BGM音量変更イベント
class ChangeVolumeEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeVolumeEvent, rapidjson::Value&)
private:
    string fileName {};
    float volume { 1.f };
private:
    ChangeVolumeEvent():fileName("") {FUNCLOG};
    ~ChangeVolumeEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};


// SE再生イベント
class PlaySEEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(PlaySEEvent, rapidjson::Value&)
private:
    string fileName {};
    float volume { 1.f };
private:
    PlaySEEvent():fileName("") {FUNCLOG};
    ~PlaySEEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};


#endif /* defined(__LastSupper__SoundEvent__) */
