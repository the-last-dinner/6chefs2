//
//  ModalLayerEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#ifndef __LastSupper__MessageEvent__
#define __LastSupper__MessageEvent__

#include "Event/GameEvent.h"

class CharacterMessageData;
class StoryMessageData;
class SystemMessageData;

class ModalLayerEvent : public GameEvent
{
// インスタンスメソッド
protected:
    ModalLayerEvent() {};
    virtual ~ModalLayerEvent() {};
    virtual bool init(rapidjson::Value& json) override;
};

// キャラクターメッセージイベント
class CharacterMessage : public ModalLayerEvent
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(CharacterMessage, rapidjson::Value&)

// インスタンス変数
private:
    queue<CharacterMessageData*> _datas {};

// インスタンスメソッド
private:
    CharacterMessage() {FUNCLOG};
    ~CharacterMessage() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    void run() override;
};

// ストーリメッセージイベント
class StoryMessage : public ModalLayerEvent
{
public:
    CREATE_FUNC_WITH_PARAM(StoryMessage, rapidjson::Value&)
    
private:
    string _title {};
    queue<StoryMessageData*> _datas {};
    
private:
    StoryMessage() {FUNCLOG};
    ~StoryMessage() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// システムメッセージイベント
class SystemMessage : public ModalLayerEvent
{
public:
    CREATE_FUNC_WITH_PARAM(SystemMessage, rapidjson::Value&)

private:
    queue<SystemMessageData*> _datas {};
    
private:
    SystemMessage() {FUNCLOG};
    ~SystemMessage() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// 画像を表示
class DispImageEvent : public ModalLayerEvent
{
public:
    CREATE_FUNC_WITH_PARAM(DispImageEvent, rapidjson::Value&)
private:
    string _fileName {};
    float _duration { 0.f };
private:
    DispImageEvent() {FUNCLOG};
    ~DispImageEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

// DisplaySaveイベント
class SaveDataSelector;
class DisplaySaveMenu : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(DisplaySaveMenu, rapidjson::Value&);
private:
    SaveDataSelector* _saveMenu { nullptr };
private:
    DisplaySaveMenu() { FUNCLOG };
    ~DisplaySaveMenu() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
    void display();
    void onExitedSaveMenu();
};

// 動画再生
class PlayVideoEvent : public ModalLayerEvent
{
public:
    CREATE_FUNC_WITH_PARAM(PlayVideoEvent, rapidjson::Value&)
private:
    string _fileName {};
    bool _skip { false };
private:
    PlayVideoEvent() {FUNCLOG};
    ~PlayVideoEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

#endif /* defined(__LastSupper__MessageEvent__) */
