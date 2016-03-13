//
//  EventListenerKeyboardManager.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/03/12.
//
//

#ifndef EventListenerKeyboardManager_h
#define EventListenerKeyboardManager_h

#include "define.h"

class EventListenerKeyboardLayer;

class EventListenerKeyboardManager
{
// クラスメソッド
public:
    static EventListenerKeyboardManager* getInstance();
    static void destroy();
    
// インスタンス変数
private:
    vector<EventListenerKeyboardLayer*> listeners {};
    
// シングルトンであるためにprivateに
private:
    EventListenerKeyboardManager();                                                         // コンストラクタ
    EventListenerKeyboardManager(const EventListenerKeyboardManager& other);				// デストラクタ
    EventListenerKeyboardManager& operator = (const EventListenerKeyboardManager& other);	// 代入演算子
    ~EventListenerKeyboardManager();                                                        // デストラクタ
    
// インスタンスメソッド
public:
    void addEventListener(EventListenerKeyboardLayer* listener);
    void removeEventListener(EventListenerKeyboardLayer* listener);
    void pauseAllEventListener(const bool paused);
};

#endif /* EventListenerKeyboardManager_h */
