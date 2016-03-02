//
//  NotificationBand.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/27.
//
//

#include "UI/NotificationBand.h"

bool NotificationBand::init(const string& message)
{
    if (!Node::init()) return false;
    
    // メッセージ表示帯
    Sprite* band = Sprite::create();
    band->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/4));
    band->setColor(Color3B::BLACK);
    band->setPosition(WINDOW_CENTER);
    band->setScaleY(0.0f);
    band->setOpacity(0);
    this->addChild(band);
    this->band = band;
    
    // 完了メッセージラベル
    Label* messageLabel = Label::createWithTTF(message, "fonts/cinecaption2.28.ttf", band->getContentSize().height / 5);
    messageLabel->setPosition(Point(messageLabel->getContentSize().width / 2 + (WINDOW_WIDTH - messageLabel->getContentSize().width)/2, band->getContentSize().height / 2));
    messageLabel->setOpacity(0);
    band->addChild(messageLabel);
    this->messageLabel = messageLabel;
    
    return true;
}

// 帯の色を変更
void NotificationBand::setBandColor(const Color3B &color)
{
    this->band->setColor(color);
}

// メッセージの色を変更
void NotificationBand::setMessageColor(const Color3B &color)
{
    this->messageLabel->setColor(color);
}

// 表示アニメーション
void NotificationBand::show(const function<void()>& callback)
{
    FiniteTimeAction* display {Spawn::create(
                                             FadeIn::create(0.2f),
                                             EaseCubicActionOut::create(ScaleTo::create(0.5f, 1.f, 1.f)),
                                             TargetedAction::create(this->messageLabel, FadeIn::create(0.4f)),
                                             nullptr
                                            )};
    if (callback)
    {
        this->band->runAction(Sequence::createWithTwoActions(display, CallFunc::create(callback)));
    }
    else
    {
        this->band->runAction(display);
    }
}

// 非表示アニメーション
void NotificationBand::hide(const function<void()>& callback)
{
    FiniteTimeAction* display {Spawn::create(
                                             EaseCubicActionOut::create(FadeOut::create(0.4f)),
                                             EaseCubicActionOut::create(ScaleTo::create(0.5f, 1.f, 0.f)),
                                             EaseCubicActionOut::create(TargetedAction::create(this->messageLabel, FadeOut::create((0.4f)))),
                                             nullptr
                                             )};
    if (callback)
    {
        this->band->runAction(Sequence::createWithTwoActions(display, CallFunc::create(callback)));
    }
    else
    {
        this->band->runAction(display);
    }
}