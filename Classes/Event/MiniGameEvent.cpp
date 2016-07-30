//
//  MiniGameEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/06.
//
//

#include "Event/MiniGameEvent.h"

#include "Event/EventFactory.h"
#include "Event/EventScriptMember.h"
#include "Event/EventScriptValidator.h"

#include "Layers/Dungeon/ButtonMashingLayer.h"
#include "Layers/Dungeon/SelectEventLayer.h"
#include "Layers/Dungeon/PasswordEventLayer.h"
#include "Layers/Message/CharacterMessagelayer.h"

#include "Datas/Message/CharacterMessageData.h"

#include "Managers/DungeonSceneManager.h"

#include "Models/StopWatch.h"

#include "Scenes/DungeonScene.h"

#pragma mark ButtonMashingEvent

ButtonMashingEvent::~ButtonMashingEvent()
{
    FUNCLOG
    
    CC_SAFE_RELEASE_NULL(this->clickCallbackEvent);
}

bool ButtonMashingEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // 連打回数
    if(!this->validator->hasMember(json, member::TIMES)) return false;
    this->count = json[member::TIMES].GetInt();
    
    // 制限時間
    if(!this->validator->hasMember(json, member::LIMIT)) return false;
    this->limit = json[member::LIMIT].GetDouble();
    
    // 成功時イベント
    if(this->validator->hasMember(json, member::TRUE_))
    {
        if(json[member::TRUE_].IsString()) this->sEventId = stoi(json[member::TRUE_].GetString());
        if(json[member::TRUE_].IsArray()) this->sEvent = this->factory->createGameEvent(json[member::TRUE_]);
        CC_SAFE_RETAIN(this->sEvent);
    }
    
    // 失敗時イベント
    if(this->validator->hasMember(json, member::FALSE_))
    {
        if(json[member::FALSE_].IsString()) this->fEventId = stoi(json[member::FALSE_].GetString());
        if(json[member::FALSE_].IsArray()) this->fEvent = this->factory->createGameEvent(json[member::FALSE_]);
        CC_SAFE_RETAIN(this->fEvent);
    }
    
    // クリック時のコールバックイベント
    if(this->validator->hasMember(json, member::EVENT))
    {
        this->clickCallbackEvent = this->factory->createGameEvent(json[member::EVENT]);
        this->clickCallbackEvent->setReusable(true);
        CC_SAFE_RETAIN(this->clickCallbackEvent);
    }
    
    return true;
}

void ButtonMashingEvent::run()
{
    ButtonMashingLayer* layer { ButtonMashingLayer::create(this->count, this->limit, [this]
    {
        DungeonSceneManager::getInstance()->runEventAsync(this->clickCallbackEvent);
    }, [this](ButtonMashingLayer::Result result)
    {
        if(result == ButtonMashingLayer::Result::SUCCESS)
        {
            if(this->sEvent)
            {
                this->event = this->sEvent;
                this->sEvent = nullptr;
                CC_SAFE_RELEASE_NULL(this->fEvent);
            }
            else
            {
                // ID指定の場合
                DungeonSceneManager::getInstance()->pushEventFront(this->sEventId);
            }
        }
        else
        {
            if(this->fEvent)
            {
                this->event = this->fEvent;
                this->fEvent = nullptr;
                CC_SAFE_RELEASE_NULL(this->sEvent);
            }
            else
            {
                // ID指定の場合
                DungeonSceneManager::getInstance()->pushEventFront(this->fEventId);
            }
        }
        
        // イベント実行
        DungeonSceneManager::getInstance()->pushEventFront(this->event);
        
        this->setDone();
    }) };
    
    if(!layer)
    {
        this->setDone();
        
        return;
    }
    
    DungeonSceneManager::getInstance()->getScene()->addChild(layer, Priority::BUTTON_MASHING_LAYER);
}

#pragma mark -
#pragma mark SelectEvent

bool SelectEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // 質問文
    if(!this->validator->hasMember(json, member::TEXT)) return false;
    this->message = json[member::TEXT][0].GetString();
    
    // 選択肢
    if(!this->validator->hasMember(json, member::CHOICES) || !json[member::CHOICES].IsArray()) return false;
    rapidjson::Value& choicesJson { json[member::CHOICES] };
    for(int i { 0 }; i < choicesJson.Size(); i++)
    {
        rapidjson::Value& choiceJson {choicesJson[i]};
        
        // 選択肢の表示ラベル
        if(!this->validator->hasMember(choiceJson, member::CHOICE)) return false;
        this->choices.push_back(choiceJson[member::CHOICE].GetString());
        
        // 選択肢のコールバックイベント
        int eventId { static_cast<int>(EventID::UNDIFINED) };
        GameEvent* event { nullptr };
        if(this->validator->hasMember(choiceJson, member::ACTION)) event = this->factory->createGameEvent(choiceJson[member::ACTION]);
        if(this->validator->hasMember(choiceJson, member::EVENT_ID)) eventId = stoi(choiceJson[member::EVENT_ID].GetString());
        CC_SAFE_RETAIN(event);
        this->eventCallBacks.push_back(SelectCallBack({eventId, event}));
    }
    
    // キャラメッセージの時
    if(this->validator->hasMember(json, member::CHARA_ID))
    {
        queue<string> pages {};
        
        pages.push(this->message);
        
        CharacterMessageData* data {CharacterMessageData::create(pages)};
        CC_SAFE_RETAIN(data);
        
        // キャラID
        if(this->validator->hasMember(json, member::CHARA_ID)) data->setCharaId(stoi(json[member::CHARA_ID].GetString()));
        
        // キャラ名
        string charaName {};
        
        if(this->validator->hasMember(json, member::NAME))
        {
            charaName = json[member::NAME].GetString();
        }
        else
        {
            charaName = CsvDataManager::getInstance()->getCharacterData()->getName(data->getCharaId());
        }
        data->setCharaName(charaName);
        
        // 画像ID
        if(this->validator->hasMember(json, member::IMG_ID)) data->setImgId(stoi(json[member::IMG_ID].GetString()));
        
        this->datas.push(data);
    }
    
    return true;
}

void SelectEvent::run()
{
    SelectEventLayer* layer { SelectEventLayer::create(this->message, this->choices, this->datas) };
    
    // コールバック
    layer->onSelected = [this](const int idx)
    {
        // 選ばれた選択肢のインデックスからコールバックを決定
        SelectCallBack callback {this->eventCallBacks.at(idx)};
        
        // コールバック実行
        DungeonSceneManager::getInstance()->pushEventFront(callback.first);
        DungeonSceneManager::getInstance()->pushEventFront(callback.second);
        
        // 選択されたコールバックイベント以外をリリース
        for(int i { 0 }; i < this->eventCallBacks.size(); i++)
        {
            if(i == idx) continue;
            
            CC_SAFE_RELEASE(this->eventCallBacks.at(i).second);
        }
        
        this->setDone();
    };
    
    DungeonSceneManager::getInstance()->getScene()->addChild(layer, Priority::SELECT_LAYER);
}

#pragma mark -
#pragma mark PasswordEvent

bool PasswordEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // 正解のパスワード
    this->password = json[member::PASSWORD].GetString();
    
    // 成功時イベント
    if(this->validator->hasMember(json, member::TRUE_))
    {
        if(json[member::TRUE_].IsString()) this->sEventId = stoi(json[member::TRUE_].GetString());
        if(json[member::TRUE_].IsArray()) this->sEvent = this->factory->createGameEvent(json[member::TRUE_]);
        CC_SAFE_RETAIN(this->sEvent);
    }
    
    // 失敗時イベント
    if(this->validator->hasMember(json, member::FALSE_))
    {
        if(json[member::FALSE_].IsString()) this->fEventId = stoi(json[member::FALSE_].GetString());
        if(json[member::FALSE_].IsArray()) this->fEvent = this->factory->createGameEvent(json[member::FALSE_]);
        CC_SAFE_RETAIN(this->fEvent);
    }
    
    return true;
}

void PasswordEvent::run()
{
    PasswordEventLayer* pLayer {PasswordEventLayer::create(this->password, [this](PasswordEventLayer::Result result)
    {
        this->setDone();
        
        if(result == PasswordEventLayer::Result::SUCCESS)
        {
            if(this->sEvent)
            {
                this->event = this->sEvent;
                this->sEvent = nullptr;
                CC_SAFE_RELEASE_NULL(this->fEvent);
            }
            else
            {
                // ID指定の場合
                DungeonSceneManager::getInstance()->pushEventFront(this->sEventId);
            }
        }
        else
        {
            if(this->fEvent)
            {
                this->event = this->fEvent;
                this->fEvent = nullptr;
                CC_SAFE_RELEASE_NULL(this->sEvent);
            }
            else
            {
                // ID指定の場合
                DungeonSceneManager::getInstance()->pushEventFront(this->fEventId);
            }
        }
        // イベント実行
        DungeonSceneManager::getInstance()->pushEventFront(this->event);
    })};
    
    if(!pLayer)
    {
        this->setDone();
        
        return;
    }
    
    DungeonSceneManager::getInstance()->getScene()->addChild(pLayer, Priority::SELECT_LAYER);
}

#pragma mark -
#pragma mark CountDown

bool CountDownEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // 制限時間
    if(!this->validator->hasMember(json, member::SECOND)) return false;
    this->second = json[member::SECOND].GetDouble();
    
    // conditionを保存
    if (this->validator->hasMember(json, member::CONDITION))
    {
        this->equip = stoi(json[member::CONDITION][0][member::EQUIP][0].GetString());
        this->checkEquip = true;
    }
    
    // 成功時イベント
    if(this->validator->hasMember(json, member::TRUE_))
    {
        if(json[member::TRUE_].IsString()) this->sEventId = stoi(json[member::TRUE_].GetString());
        if(json[member::TRUE_].IsArray()) this->sEvent = this->factory->createGameEvent(json[member::TRUE_]);
        CC_SAFE_RETAIN(this->sEvent);
    }
    
    // 失敗時イベント
    if (this->validator->hasMember(json, member::FALSE_))
    {
        if(json[member::FALSE_].IsString()) this->fEventId = stoi(json[member::FALSE_].GetString());
        if(json[member::FALSE_].IsArray()) this->fEvent = this->factory->createGameEvent(json[member::FALSE_]);
        CC_SAFE_RETAIN(this->fEvent);
    }
    if (this->validator->hasMember(json, member::ACTION))
    {
        if(json[member::ACTION].IsString()) this->fEventId = stoi(json[member::ACTION].GetString());
        if(json[member::ACTION].IsArray()) this->fEvent = this->factory->createGameEvent(json[member::ACTION]);
        CC_SAFE_RETAIN(this->fEvent);
    }
    
    return true;
}

void CountDownEvent::run()
{
    StopWatch* stopWatch = DungeonSceneManager::getInstance()->getStopWatch();
    
    stopWatch->setCountDown(this);
    
    stopWatch->scheduleCallback = [this](double time)
    {
        // 条件チェック
        bool condition = false;
        if(this->checkEquip)
        {
            condition = PlayerDataManager::getInstance()->getLocalData()->isEquipedItem(this->equip);
        }
        
        // 条件を満たしていた場合
        if (condition)
        {
            CC_SAFE_RELEASE_NULL(this->fEvent);
            if(this->sEvent)
            {
                DungeonSceneManager::getInstance()->pushEventFront(this->sEvent);
            }
            else
            {
                // ID指定の場合
                DungeonSceneManager::getInstance()->pushEventFront(this->sEventId);
            }
            DungeonSceneManager::getInstance()->runEventQueue();
            return false;
        }
        CCLOG("COUNT DOWN >> %f", this->second - time);
        // 時間切れチェック
        if (static_cast<float>(time) >= this->second)
        {
            CC_SAFE_RELEASE_NULL(this->sEvent);
            if(this->fEvent)
            {
                DungeonSceneManager::getInstance()->pushEventFront(this->fEvent);
            }
            else
            {
                // ID指定の場合
                DungeonSceneManager::getInstance()->pushEventFront(this->fEventId);
            }
            DungeonSceneManager::getInstance()->runEventQueue();
            return false;
        }
        return true;
    };
    
    // カウントダウンスタート
    stopWatch->startCountDown(0.5f);
    
    this->setDone();
}

#pragma mark -
#pragma mark StopCount

bool StopCountEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    return true;
}

void StopCountEvent::run()
{
    DungeonSceneManager::getInstance()->pauseStopWatch();
    DungeonSceneManager::getInstance()->releaseStopWatch();
    this->setDone();
}
