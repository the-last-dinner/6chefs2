//
//  FlagEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/27.
//
//

#ifndef __LastSupper__FlagEvent__
#define __LastSupper__FlagEvent__

#include "Event/GameEvent.h"

// 実行中IDのイベントを発動させないようにする
class NeverAgainEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(NeverAgainEvent, rapidjson::Value&)
private:
    NeverAgainEvent() { FUNCLOG };
    ~NeverAgainEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// アイテムを入手する
class GetItemEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(GetItemEvent, rapidjson::Value&)
private:
    int _itemId { 0 };
private:
    GetItemEvent() { FUNCLOG };
    ~GetItemEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// アイテムを破棄する
class RemoveItemEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(RemoveItemEvent, rapidjson::Value&)
private:
    int _itemId { 0 };
private:
    RemoveItemEvent() { FUNCLOG };
    ~RemoveItemEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// キャラクタのプロフィールを追加
class AddProfileEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(AddProfileEvent, rapidjson::Value&)
private:
    int _charaId { static_cast<int>(CharacterID::UNDIFINED) };
    int _infoLevel { 0 };
private:
    AddProfileEvent() { FUNCLOG };
    ~AddProfileEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// チャプターを切り替える
class ChangeChapterEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeChapterEvent, rapidjson::Value&)
private:
    int _chapterId { 0 };
private:
    ChangeChapterEvent() { FUNCLOG };
    ~ChangeChapterEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// キャラクタの好感度を変更
class ChangeLikabilityRatingEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeLikabilityRatingEvent, rapidjson::Value&)
private:
    int _charaId { static_cast<int>(CharacterID::UNDIFINED) };
    int _rating { 0 };
private:
    ChangeLikabilityRatingEvent() { FUNCLOG };
    ~ChangeLikabilityRatingEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// イベントステータスの変更
class ChangeEventStatusEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(ChangeEventStatusEvent, rapidjson::Value&)
private:
    int _status {0};
    int _mapId {-1};
    int _eventId {-1};
private:
    ChangeEventStatusEvent() { FUNCLOG };
    ~ChangeEventStatusEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// トロフィーの取得
class GetTrophyEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(GetTrophyEvent, rapidjson::Value&)
private:
    int _trophyId {0};
private:
    GetTrophyEvent() { FUNCLOG };
    ~GetTrophyEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// アイテムの引き渡し
class GiveItemEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(GiveItemEvent, rapidjson::Value&)
private:
    int _fromCharaId { -1 };
    int _toCharaId { -1 };
private:
    GiveItemEvent() {FUNCLOG};
    ~GiveItemEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

class SetGameClearEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(SetGameClearEvent, rapidjson::Value&)
private:
    SetGameClearEvent() { FUNCLOG };
    ~SetGameClearEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

#endif /* defined(__LastSupper__FlagEvent__) */
