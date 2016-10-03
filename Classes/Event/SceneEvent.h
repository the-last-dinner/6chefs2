//
//  SceneEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#ifndef __LastSupper__SceneEvent__
#define __LastSupper__SceneEvent__

#include "Event/GameEvent.h"

// マップ移動イベント
class ChangeMapEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeMapEvent, rapidjson::Value&)
private:
    Location destLocation {};
    Location currentLocation {};
    int initEventId { etoi(EventID::UNDIFINED) };
private:
    ChangeMapEvent() {FUNCLOG};
    ~ChangeMapEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run();
};

// waitイベント
class WaitEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(WaitEvent, rapidjson::Value&)
private:
    float duration {0.f};
private:
    WaitEvent() {FUNCLOG};
    ~WaitEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
    virtual void update(float delta) override;
};

// FadeOutイベント
class FadeOutEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(FadeOutEvent, rapidjson::Value&)
private:
    Color3B color { Color3B::BLACK };
    float duration { 0.5f };
private:
    FadeOutEvent() {FUNCLOG};
    ~FadeOutEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// FadeInイベント
class FadeInEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(FadeInEvent, rapidjson::Value&)
private:
    float duration { 0.5f };
private:
    FadeInEvent() {FUNCLOG};
    ~FadeInEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// ゲームオーバーシーンへ移動
class GameOverEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(GameOverEvent, rapidjson::Value&)
private:
    int gameOverId {0};
private:
    GameOverEvent() {FUNCLOG};
    ~GameOverEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// エンディングシーンへ移動
class EndingEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(EndingEvent, rapidjson::Value&)
private:
    int endingId {0};
private:
    EndingEvent() {FUNCLOG};
    ~EndingEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// タイトルへ移動
class BackToTitleEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(BackToTitleEvent, rapidjson::Value&)
private:
    BackToTitleEvent() {FUNCLOG};
    ~BackToTitleEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// デバッグ用InfoAssertを出す
class InfoAssertEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(InfoAssertEvent, rapidjson::Value&)
private:
    string text {};
private:
    InfoAssertEvent() {FUNCLOG};
    ~InfoAssertEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

#endif /* defined(__LastSupper__SceneEvent__) */
