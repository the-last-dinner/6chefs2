//
//  MessageLayer.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/12.
//
//

#ifndef __LastSupper__MessageLayer__
#define __LastSupper__MessageLayer__

#include "Common.h"

class MessageData;
class EventListenerKeyboardLayer;

class MessageLayer : public Layer
{
// 定数
public:
    static const float INPUT_WAIT_DURATION;
// インスタンス変数
protected:
    Sprite* frame { nullptr };
private:
    EventListenerKeyboardLayer* listener { nullptr };
    function<void()> onClose { nullptr };
    Label* message { nullptr };
    bool allLetterDisplayed {false};
    float latency { 0.05f };
 
// インスタンスメソッド
public:
    void onEnterKeyPressed();
protected:
    MessageLayer();
    virtual ~MessageLayer();
    virtual bool init(function<void()> onCloseCallback);
private:
    virtual Label* createMessage() = 0;
    virtual bool hasNextPage() = 0;
    void displayMessageWithAnimation(Label* message);
    void displayMessage(Label* message);
    
    void onAllLetterDisplayed();
    
    void nextPage();
    
    void close();
};

#endif /* defined(__LastSupper__MessageLayer__) */
