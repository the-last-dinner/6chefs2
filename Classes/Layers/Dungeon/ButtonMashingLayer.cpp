//
//  ButtonMashingLayer.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/10.
//
//

#include "Layers/Dungeon/ButtonMashingLayer.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// create関数
ButtonMashingLayer* ButtonMashingLayer::create(int time, float limit, ResultCallback callback)
{
    ButtonMashingLayer* p { new(nothrow) ButtonMashingLayer() };
    if(p && p->init(time, limit, callback))
    {
        CC_SAFE_RETAIN(p);
        return p;
    }
    else
    {
        delete p;
        p = nullptr;
        return nullptr;
    }
}

// コンストラクタ
ButtonMashingLayer::ButtonMashingLayer() {FUNCLOG};

// デストラクタ
ButtonMashingLayer::~ButtonMashingLayer() {FUNCLOG};

// 初期化
bool ButtonMashingLayer::init(int time, float limit, ResultCallback callback)
{
    if(!Layer::init() || time == 0 || limit == 0.f || !callback) return false;
    
    this->count = time;
    this->callback = callback;
    
    // 文字表示
    Label* label { Label::createWithTTF("スペース連打！！", "fonts/cinecaption2.28.ttf", 25) };
    label->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 4));
    label->setCascadeOpacityEnabled(true);
    this->addChild(label);
    
    // 文字のアニメーション設定
    label->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3f), FadeIn::create(0.3f), nullptr)));
    
    // イベントリスナ生成
    EventListenerKeyboardLayer* listener { EventListenerKeyboardLayer::create() };
    listener->onSpaceKeyPressed = CC_CALLBACK_0(ButtonMashingLayer::onSpaceKeyPressed, this);
    this->addChild(listener);
    
    // 指定時間後に失敗通知
    this->runAction(Sequence::create(DelayTime::create(limit), CallFunc::create([callback]{callback(Result::FAILURE);}), RemoveSelf::create(), nullptr));
    
    return true;
}

// スペースキーが押された時
void ButtonMashingLayer::onSpaceKeyPressed()
{
    this->count--;
    
    if(this->count == 0 && this->callback)
    {
        this->stopAllActions();
        this->callback(Result::SUCCESS);
        this->runAction(Sequence::create(FadeOut::create(0.3f), RemoveSelf::create(), nullptr));
    }
}