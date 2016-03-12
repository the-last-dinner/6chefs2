//
//  EventListenerKeyboardManager.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/03/12.
//
//

#include "Managers/EventListenerKeyboardManager.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// 唯一のインスタンス
static EventListenerKeyboardManager* _instance { nullptr };

// インスタンスを取得
EventListenerKeyboardManager* EventListenerKeyboardManager::getInstance()
{
    if(!_instance) _instance = new EventListenerKeyboardManager();
    
    return _instance;
}

// インスタンスを破棄
void EventListenerKeyboardManager::destroy()
{
    delete _instance;
}

// コンストラクタ
EventListenerKeyboardManager::EventListenerKeyboardManager() { FUNCLOG };

// デストラクタ
EventListenerKeyboardManager::~EventListenerKeyboardManager() { FUNCLOG };

// リスナーを追加
void EventListenerKeyboardManager::addEventListener(EventListenerKeyboardLayer* listener)
{
    this->listeners.push_back(listener);
}

// リスナーを削除
void EventListenerKeyboardManager::removeEventListener(EventListenerKeyboardLayer* listener)
{
    this->listeners.erase(remove(this->listeners.begin(), this->listeners.end(), listener));
}

// 登録されているリスナーを全て一時停止
void EventListenerKeyboardManager::pauseAllEventListener(const bool paused)
{
    for(EventListenerKeyboardLayer* listener : this->listeners)
    {
        listener->setPaused(paused);
    }
}
