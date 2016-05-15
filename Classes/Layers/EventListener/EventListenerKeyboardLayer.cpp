//
//  EventListenerKeyboardLayer.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/23.
//
//

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

#include "Managers/EventListenerKeyboardManager.h"
#include "Managers/KeyconfigManager.h"

# pragma mark private

// コンストラクタ
EventListenerKeyboardLayer::EventListenerKeyboardLayer(){ FUNCLOG }

// デストラクタ
EventListenerKeyboardLayer::~EventListenerKeyboardLayer()
{
    FUNCLOG
    
    EventListenerKeyboardManager::getInstance()->removeEventListener(this);
}

// 初期化
bool EventListenerKeyboardLayer::init()
{
    if(!Layer::init()) return false;
    
    EventListenerKeyboard* listenerKeyboard { EventListenerKeyboard::create() };
    listenerKeyboard->onKeyPressed = CC_CALLBACK_1(EventListenerKeyboardLayer::onKeyPressed, this);
    listenerKeyboard->onKeyReleased = CC_CALLBACK_1(EventListenerKeyboardLayer::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerKeyboard, this);
    this->listenerKeyboard = listenerKeyboard;
    
    // 管理クラスに登録
    EventListenerKeyboardManager::getInstance()->addEventListener(this);
    
    return true;
}

// キーを押し続けている時
void EventListenerKeyboardLayer::intervalCheck(float duration)
{
    if(this->pressingKeys.empty())
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck));
        
        return;
    }
    
    if(this->paused) return;
    
    if(this->intervalInputCheck) this->intervalInputCheck(this->pressingKeys);
}

void EventListenerKeyboardLayer::scheduleIntervalCheck()
{
    if(this->isScheduled(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck))) this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck));
    if(this->intervalInputCheck) this->schedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck), this->interval, CC_REPEAT_FOREVER, this->delay);
}

// キーコードを変換。ゲームで使わないキーが与えられた場合はSIZEを返す
Key EventListenerKeyboardLayer::convertKeyCode(const EventKeyboard::KeyCode& keyCode)
{
    return KeyconfigManager::getInstance()->convertKeyCode(keyCode);
}

# pragma mark -
# pragma mark getter

// 指定のキーが押し状態か判別
bool EventListenerKeyboardLayer::isPressed(const Key& key) const
{
    if(this->keyStatus.count(key) == 0) return false;
    
    return this->keyStatus.at(key);
}

// 押している方向キーを取得
const vector<Key>& EventListenerKeyboardLayer::getPressedCursorKeys() const
{
    return this->pressingKeys;
}

# pragma mark -
# pragma mark public

// リスナを有効/無効化
void EventListenerKeyboardLayer::setEnabled(bool enabled)
{
    this->releaseKeyAll();
    this->listenerKeyboard->setEnabled(enabled);
}

// キーを押した瞬間から初回キー入力確認までの時間を設定
void EventListenerKeyboardLayer::setInputCheckDelay(float delay)
{
    this->delay = delay;
}

// キー入力の確認間隔を設定
void EventListenerKeyboardLayer::setInputCheckInterval(float interval)
{
    if(this->interval == interval) return;
    
    this->interval = interval;
    
    // すでにスケジュールされていたら、一旦スケジュール停止して新たなインターバルで再開する
    if(this->isScheduled(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck)))
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck));
        this->schedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck), interval);
    }
}

// キーを押した時
void EventListenerKeyboardLayer::onKeyPressed(const EventKeyboard::KeyCode& keyCode)
{
    Key key {this->convertKeyCode(keyCode)};
    if(key == Key::SIZE) return;
    this->keyStatus[key] = true;
    switch (key) {
        case Key::UP:
        case Key::DOWN:
        case Key::LEFT:
        case Key::RIGHT:
            if(this->onCursorKeyPressed && !this->paused) this->onCursorKeyPressed(key);
            this->pressingKeys.push_back(key);
            // 方向キーを押した時は、入力チェック用にスケジューリング
            this->scheduleIntervalCheck();
            break;
            
        case Key::ENTER:
            if(this->onEnterKeyPressed && !this->paused) this->onEnterKeyPressed();
            break;
            
        case Key::MENU:
            if(this->onMenuKeyPressed && !this->paused) this->onMenuKeyPressed();
            break;
            
        case Key::DASH:
            if(this->onDashKeyPressed && !this->paused) this->onDashKeyPressed();
            break;
            
        case Key::KEY_CONF:
            if(this->onKeyConfKeyPressed && !this->paused) this->onKeyConfKeyPressed();
            break;
            
        default: break;
    }
}

// キーを離した時
void EventListenerKeyboardLayer::onKeyReleased(const EventKeyboard::KeyCode& keyCode) { this->releaseKey(this->convertKeyCode(keyCode)); }

//　キーを離すとき
void EventListenerKeyboardLayer::releaseKey(const Key& key)
{
    if(key == Key::SIZE) return;
    this->keyStatus[key] = false;
    if(find(this->pressingKeys.begin(), this->pressingKeys.end(), key) != this->pressingKeys.end()) this->pressingKeys.erase(remove(this->pressingKeys.begin(), this->pressingKeys.end(), key));
}

// 全てのキーを強制リリースする
void EventListenerKeyboardLayer::releaseKeyAll()
{
    for(int key = 0; key < static_cast<int>(Key::SIZE); key++)
    {
        this->releaseKey(static_cast<Key>(key));
    }
}

void EventListenerKeyboardLayer::setPaused(bool paused)
{
    this->paused = paused;
    
    if(paused)
    {
        this->unschedule(CC_SCHEDULE_SELECTOR(EventListenerKeyboardLayer::intervalCheck));
    }
    else
    {
        if(this->pressingKeys.empty()) return;
        this->scheduleIntervalCheck();
    }
}

void EventListenerKeyboardLayer::clearCallbacks()
{
    this->onCursorKeyPressed = nullptr;
    this->onEnterKeyPressed = nullptr;
    this->onMenuKeyPressed = nullptr;
    this->onDashKeyPressed = nullptr;
    this->onKeyConfKeyPressed = nullptr;
    this->intervalInputCheck = nullptr;
}
