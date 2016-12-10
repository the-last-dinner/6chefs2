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
    string _fileName {};
    float _volume { 1.f };
private:
    PlayBGMEvent() { FUNCLOG };
    ~PlayBGMEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// BGM停止イベント
class StopBGMEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(StopBGMEvent, rapidjson::Value&)
private:
    string _fileName {};
private:
    StopBGMEvent() { FUNCLOG };
    ~StopBGMEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// BGM音量変更イベント
class ChangeVolumeEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeVolumeEvent, rapidjson::Value&)
private:
    string _fileName {};
    float _volume { 1.f };
private:
    ChangeVolumeEvent() { FUNCLOG };
    ~ChangeVolumeEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};


// SE再生イベント
class PlaySEEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(PlaySEEvent, rapidjson::Value&)
private:
    string _fileName {};
    float _volume { 1.f };
private:
    PlaySEEvent() { FUNCLOG };
    ~PlaySEEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// Voice再生イベント
class PlayVoiceEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(PlayVoiceEvent, rapidjson::Value&)
private:
    string _fileName {};
    float _volume { 1.f };
private:
    PlayVoiceEvent() { FUNCLOG };
    ~PlayVoiceEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};


#endif /* defined(__LastSupper__SoundEvent__) */
