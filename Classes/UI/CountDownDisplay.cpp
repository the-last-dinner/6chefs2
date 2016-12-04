//
//  CountDown.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/12/03.
//
//

#include "UI/CountDownDisplay.h"

// 定数
const float CountDownDisplay::HORIZONTAL_MARGIN {40.f};
const float CountDownDisplay::VERTICAL_MARGIN {40.f};
const float CountDownDisplay::WARNING_COLOR_CHANGE_THRESHOLD { 33.f };
const Color3B CountDownDisplay::NORMAL_COLOR { Color3B::WHITE };
const Color3B CountDownDisplay::WARNING_COLOR { Color3B::RED };

// コンストラクタ
CountDownDisplay::CountDownDisplay() {FUNCLOG};

// デストラクタ
CountDownDisplay::~CountDownDisplay() {FUNCLOG};

// 初期化
bool CountDownDisplay::init()
{
    // 下地のディスプレイ
    Sprite* display {Sprite::create()};
    display->setTextureRect(Rect(0, 0, WINDOW_WIDTH / 6, WINDOW_HEIGHT / 8));
    display->setColor(Color3B::BLACK);
    display->setOpacity(50);
    this->addChild(display);
    
    // カウントダウンラベル
    Label* secondsLabel {Label::createWithTTF("", Resource::Font::SYSTEM, 40.f)};
    secondsLabel->setPosition(display->getContentSize().width / 2, display->getContentSize().height / 2);
    secondsLabel->setColor(Color3B::WHITE);
    display->addChild(secondsLabel);
    this->secondsLabel = secondsLabel;
    
    Point inPosition {Point(WINDOW_WIDTH - display->getContentSize().width / 2 - HORIZONTAL_MARGIN, WINDOW_HEIGHT - display->getContentSize().height / 2 - VERTICAL_MARGIN)};
    Point outPosition {Point(WINDOW_WIDTH - display->getContentSize().width / 2 - HORIZONTAL_MARGIN, WINDOW_HEIGHT + display->getContentSize().height)};
    
    if(!SlideNode::init(inPosition, outPosition)) return false;
    
    return true;
}

// 残り秒数をセット
void CountDownDisplay::setSecondsLeft(const float& secondsLeft)
{
    char secondsLeftCharas[10];
    sprintf(secondsLeftCharas, "%.2f", secondsLeft);
    string secondsLeftString = secondsLeftCharas;
    this->secondsLabel->setString(secondsLeftString);
    // this->changeColor();
}

// 色を変更
void CountDownDisplay::changeColor(const float percentage)
{
    if(percentage < WARNING_COLOR_CHANGE_THRESHOLD)
    {
        this->secondsLabel->setColor(WARNING_COLOR);
    }
    else
    {
        this->secondsLabel->setColor(NORMAL_COLOR);
    }
}

// 表示
void CountDownDisplay::slideIn(AnimationCallback cb)
{
    this->stopAllActions();
    this->runAction(Spawn::createWithTwoActions(EaseElasticOut::create(MoveTo::create(this->slideTime, this->inPosition), 0.5f), FadeIn::create(this->slideTime)));
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(this->slideTime), CallFunc::create([this, cb]{if(cb) cb(this);})));
}

// 非表示
void CountDownDisplay::slideOut(AnimationCallback cb)
{
    this->stopAllActions();
    this->runAction(Spawn::createWithTwoActions(EaseCubicActionIn::create(MoveTo::create(this->slideTime, this->outPosition)), FadeOut::create(this->slideTime)));
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(this->slideTime), CallFunc::create([this, cb]{if(cb) cb(this);})));
}
