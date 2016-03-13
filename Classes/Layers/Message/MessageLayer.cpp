//
//  MessageLayer.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/12.
//
//

#include "Layers/Message/MessageLayer.h"

#include "Datas/Message/MessageData.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// 定数
const float MessageLayer::INPUT_WAIT_DURATION { 0.3f };

// コンストラクタ
MessageLayer::MessageLayer() {};

// デストラクタ
MessageLayer::~MessageLayer() {};

// 初期化
bool MessageLayer::init(function<void()> onCloseCallback)
{
    if(!Layer::init()) return false;
    
    this->onClose = onCloseCallback;
    
    // イベントリスナ生成
    EventListenerKeyboardLayer* listener {EventListenerKeyboardLayer::create()};
    listener->onEnterKeyPressed = CC_CALLBACK_0(MessageLayer::onEnterKeyPressed, this);
    this->addChild(listener);
    this->listener = listener;
    
    this->setCascadeOpacityEnabled(true);
    
    // メッセージを生成して表示
    this->displayMessageWithAnimation(this->createMessage());
    
    return true;
}

// 決定キーを押した時
void MessageLayer::onEnterKeyPressed()
{
    // ページ内ですべての文字を表示し終えていたら
    if(this->allLetterDisplayed)
    {
        if(this->hasNextPage())
        {
            this->nextPage();
        }
        else
        {
            this->close();
        }
    }
    else
    {
        // 表示できていなければ、全表示
        this->displayMessage(this->message);
    }
}

// 文字を一文字ずつ表示
void MessageLayer::displayMessageWithAnimation(Label* message)
{
    // messageがnullptrなら終了
    if(!message) return;
    
    this->message = message;
    
    int stringLength = message->getStringLength();
    
    // アニメーション設定
    for(int i = 0; i < stringLength; i++)
    {
        Sprite* letter { message->getLetter(i) };
        
        // エスケープ文字をスルー
        if(letter)
        {
            letter->setOpacity(0);
            letter->runAction(Sequence::createWithTwoActions(DelayTime::create(this->latency * i), FadeIn::create(0.1f)));
        }
    }
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(stringLength * this->latency), CallFunc::create([this](){this->onAllLetterDisplayed();})));
}

// すべての文字を表示
void MessageLayer::displayMessage(Label* message)
{
    if(!message) return;
    
    for(int i = 0; i < message->getStringLength(); i++)
    {
        Sprite* letter { message->getLetter(i) };
        if(!letter) continue;
        letter->stopAllActions();
        letter->setOpacity(255);
    }
    
    this->onAllLetterDisplayed();
    
    // 全ての文字を表示してから数秒間リスナを無効にしておく
    this->listener->setEnabled(false);
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(INPUT_WAIT_DURATION), CallFunc::create([this]{this->listener->setEnabled(true);})));
}

// すべての文字を表示した時
void MessageLayer::onAllLetterDisplayed()
{
    this->allLetterDisplayed = true;
}

// 次のページへ
void MessageLayer::nextPage()
{
    this->message->removeFromParent();
    this->allLetterDisplayed = false;
    this->displayMessageWithAnimation(this->createMessage());
}

// メッセージレイヤを閉じる
void MessageLayer::close()
{
    this->message->setCascadeOpacityEnabled(true);
    this->frame->setCascadeOpacityEnabled(true);
    this->listener->setEnabled(false);
    this->runAction(FadeOut::create(0.2f));
    if (this->onClose) this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.2f), CallFunc::create([this](){this->onClose(); this->removeFromParent();})));
}
