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
#include "Event/GameEventHelper.h"

#include "Layers/Dungeon/ButtonMashingLayer.h"
#include "Layers/Dungeon/SelectEventLayer.h"
#include "Layers/Dungeon/PasswordEventLayer.h"
#include "Layers/Message/CharacterMessagelayer.h"

#include "Datas/Message/CharacterMessageData.h"

#include "Managers/DungeonSceneManager.h"

#include "Models/StopWatch.h"

#include "Scenes/DungeonScene.h"
#include "Tasks/EventTask.h"

#include "UI/CountDownDisplay.h"

#pragma mark ButtonMashingEvent

bool ButtonMashingEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // 連打回数
    if (!_eventHelper->hasMember(_json, member::TIMES)) return false;
    _count = _json[member::TIMES].GetInt();
    
    // 制限時間
    if (!_eventHelper->hasMember(_json, member::LIMIT)) return false;
    _limit = _json[member::LIMIT].GetDouble();
    
    // クリック時のコールバックイベント
    _clickCallbackEvent = _factory->createGameEvent(_json[member::EVENT], this);
    CC_SAFE_RETAIN(_clickCallbackEvent);
    
    return true;
}

void ButtonMashingEvent::run()
{
    ButtonMashingLayer* layer { ButtonMashingLayer::create(_count, _limit, [this] {
        // クリック時のコールバックイベント
        if (_clickCallbackEvent) {
            _clickCallbackEvent->run();
        }
    }, [this](ButtonMashingLayer::Result result) {
        if (result == ButtonMashingLayer::Result::SUCCESS) {
            _resultCallbackEvent = _eventHelper->createMiniGameSuccessCallbackEvent(_json, _factory, this);
        } else {
            _resultCallbackEvent = _eventHelper->createMiniGameFailureCallbackEvent(_json, _factory, this);
        }
        
        CC_SAFE_RETAIN(_resultCallbackEvent);
        _resultCallbackEvent->run();
    })};
    
    if (!layer) {
        this->setDone();
        CC_SAFE_RELEASE_NULL(_clickCallbackEvent);
        return;
    }
    
    DungeonSceneManager::getInstance()->getScene()->addChild(layer, Priority::BUTTON_MASHING_LAYER);
}

void ButtonMashingEvent::update(float delta)
{
    if (!_resultCallbackEvent) return;
    
    _resultCallbackEvent->update(delta);
    
    if (!_resultCallbackEvent->isDone()) return;
    
    CC_SAFE_RELEASE_NULL(_resultCallbackEvent);
    this->setDone();
}

#pragma mark -
#pragma mark SelectEvent

bool SelectEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // 質問文
    if (!_eventHelper->hasMember(_json, member::TEXT)) return false;
    _message = _json[member::TEXT][0].GetString();
    
    // 選択肢
    if (!_eventHelper->hasMember(_json, member::CHOICES) || !_json[member::CHOICES].IsArray()) return false;
    
    rapidjson::Value& choicesJson { _json[member::CHOICES] };
    for (int i { 0 }; i < choicesJson.Size(); i++) {
        rapidjson::Value& choiceJson { choicesJson[i] };
        
        // 選択肢の表示ラベル
        if(!_eventHelper->hasMember(choiceJson, member::CHOICE)) return false;
        _choices.push_back(choiceJson[member::CHOICE].GetString());
    }
    
    // キャラメッセージの時
    if (_eventHelper->hasMember(_json, member::CHARA_ID)) {
        queue<string> pages {};
        
        pages.push(_message);
        
        CharacterMessageData* data {CharacterMessageData::create(pages)};
        CC_SAFE_RETAIN(data);
        
        // キャラID
        if (_eventHelper->hasMember(_json, member::CHARA_ID)) data->setCharaId(stoi(_json[member::CHARA_ID].GetString()));
        
        // キャラ名
        string charaName {};
        
        if (_eventHelper->hasMember(_json, member::NAME)) {
            charaName = _json[member::NAME].GetString();
        } else {
            charaName = CsvDataManager::getInstance()->getCharacterData()->getName(data->getCharaId());
        }
        data->setCharaName(charaName);
        
        // 画像ID
        if (_eventHelper->hasMember(_json, member::IMG_ID)) data->setImgId(stoi(_json[member::IMG_ID].GetString()));
        
        _datas.push(data);
    }
    
    return true;
}

void SelectEvent::run()
{
    SelectEventLayer* layer { SelectEventLayer::create(_message, _choices, _datas) };
    
    // コールバック
    layer->onSelected = [this](const int idx) {
        // 選ばれた選択肢のインデックスからコールバックイベントを生成し実行
        GameEvent* callbackEvent { this->createSpawnFromIdOrAction(_json[member::CHOICES][idx]) };
        CC_SAFE_RETAIN(callbackEvent);
        _event = callbackEvent;
        
        (_event)? _event->run() : this->setDone();
        
    };
    
    DungeonSceneManager::getInstance()->getScene()->addChild(layer, Priority::SELECT_LAYER);
}

void SelectEvent::update(float delta){
    
    if (!_event) return;
    
    _event->update(delta);
    
    if (!_event->isDone()) return;

    CC_SAFE_RELEASE_NULL(_event);
    this->setDone();
}

void SelectEvent::stop(int code)
{
    if (_event) _event->stop(code);
}

#pragma mark -
#pragma mark PasswordEvent

bool PasswordEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // 正解のパスワード
    _password = _json[member::PASSWORD].GetString();
    
    return true;
}

void PasswordEvent::run()
{
    PasswordEventLayer* pLayer {PasswordEventLayer::create(_password, [this](PasswordEventLayer::Result result)
    {
        if (result == PasswordEventLayer::Result::SUCCESS) {
            _resultCallbackEvent = _eventHelper->createMiniGameSuccessCallbackEvent(_json, _factory, this);
        } else {
            _resultCallbackEvent = _eventHelper->createMiniGameFailureCallbackEvent(_json, _factory, this);
        }
        
        CC_SAFE_RETAIN(_resultCallbackEvent);
        _resultCallbackEvent->run();
    })};
    
    if (!pLayer) {
        this->setDone();
        return;
    }
    
    DungeonSceneManager::getInstance()->getScene()->addChild(pLayer, Priority::SELECT_LAYER);
}

void PasswordEvent::update(float delta)
{
    if (!_resultCallbackEvent) return;
    
    _resultCallbackEvent->update(delta);
    
    if (!_resultCallbackEvent->isDone()) return;
    
    CC_SAFE_RELEASE_NULL(_resultCallbackEvent);
    this->setDone();
}

#pragma mark -
#pragma mark CountDown

CountDownEvent::~CountDownEvent()
{
    FUNCLOG
    CC_SAFE_RELEASE(_successCallbackEvent);
    CC_SAFE_RELEASE(_failureCallbackEvent);
}

bool CountDownEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // 制限時間
    if (!_eventHelper->hasMember(_json, member::SECOND)) return false;
    _second = _json[member::SECOND].GetDouble();
    
    // conditionを保存
    if (_eventHelper->hasMember(_json, member::CONDITION)) {
        _equip = stoi(_json[member::CONDITION][0][member::EQUIP][0].GetString());
        _checkEquip = true;
    }
    
    if (_eventHelper->hasMember(_json, member::DISPLAY)) {
        _display = _json[member::DISPLAY].GetBool();
    }
    
    // コールバック設定
    _successCallbackEvent = _eventHelper->createMiniGameSuccessCallbackEvent(_json, _factory, this);
    _failureCallbackEvent = _eventHelper->createMiniGameFailureCallbackEvent(_json, _factory, this);;
    CC_SAFE_RETAIN(_successCallbackEvent);
    CC_SAFE_RETAIN(_failureCallbackEvent);
    
    return true;
}

void CountDownEvent::run()
{
    StopWatch* stopWatch { DungeonSceneManager::getInstance()->getStopWatch() };
    stopWatch->setCountDownEvent(this);
    this->setReusable(true);
    
    if (_display) {
        DungeonSceneManager::getInstance()->getScene()->getCountDownDisplay()->slideIn();
    }
    
    stopWatch->scheduleCallback = [this](double time) {
        
        if (_isTimeUp) return false;
        
        // 条件チェック
        bool condition = false;
        if (_checkEquip) {
            condition = PlayerDataManager::getInstance()->getLocalData()->isEquipedItem(_equip);
        }
        
        // 条件を満たしていた場合
        if (condition) {
            this->runResultCallbackEvent(_successCallbackEvent);
            return false;
        }

        // ディスプレイ表示
        if (_display) {
            CountDownDisplay* countDownDisplay { DungeonSceneManager::getInstance()->getScene()->getCountDownDisplay() };
            countDownDisplay->setSecondsLeft(_second - time);
            countDownDisplay->changeColor((_second - time) / _second * 100);
        }
        
        CCLOG("COUNT DOWN >> %f", _second - time);
        
        // 時間切れチェック
        if (static_cast<float>(time) >= _second) {
            this->runResultCallbackEvent(_failureCallbackEvent);
            return false;
        }
        
        return true;
    };
    
    // カウントダウンスタート
    stopWatch->startCountDown(0.03f);
    
    this->setDone();
}

void CountDownEvent::runResultCallbackEvent(GameEvent *callbackEvent)
{
    _isTimeUp = true;
    
    if (!callbackEvent) return;
    
    EventTask* eventTask { DungeonSceneManager::getInstance()->getEventTask() };
    eventTask->pushEventBack(callbackEvent);
    eventTask->runEventQueue();
}

#pragma mark -
#pragma mark StopCount

bool StopCountEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init(json)) return false;
    return true;
}

void StopCountEvent::run()
{
    DungeonSceneManager::getInstance()->pauseStopWatch();
    DungeonSceneManager::getInstance()->releaseStopWatch();
    DungeonSceneManager::getInstance()->getScene()->getCountDownDisplay()->slideOut();

    this->setDone();
}
