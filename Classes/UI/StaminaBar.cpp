//
//  StaminaBar.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/30.
//
//

#include "UI/StaminaBar.h"

// 定数
const float StaminaBar::HORIZONTAL_MARGIN {40.f};
const float StaminaBar::VERTICAL_MARGIN {40.f};
const float StaminaBar::WARNING_COLOR_CHANGE_THRESHOLD { 33.f };
const Color3B StaminaBar::NORMAL_COLOR { Color3B(0, 148, 122) };
const Color3B StaminaBar::WARNING_COLOR { Color3B(200, 0, 0) };

// コンストラクタ
StaminaBar::StaminaBar() {FUNCLOG};

// デストラクタ
StaminaBar::~StaminaBar() {FUNCLOG};

// 初期化
bool StaminaBar::init()
{
    // 棒
    Sprite* bar {Sprite::create()};
    bar->setTextureRect(Rect(0, 0, WINDOW_WIDTH / 2, 5));
    bar->setColor(NORMAL_COLOR);
    
    Point inPosition {Point(WINDOW_WIDTH - bar->getContentSize().width / 2 - HORIZONTAL_MARGIN, WINDOW_HEIGHT - bar->getContentSize().height / 2 - VERTICAL_MARGIN)};
    Point outPosition {Point(WINDOW_WIDTH - bar->getContentSize().width / 2 - HORIZONTAL_MARGIN, WINDOW_HEIGHT + bar->getContentSize().height)};
    
    if(!SlideNode::init(inPosition, outPosition)) return false;
    
    // カスケード
    this->setCascadeOpacityEnabled(true);
    
    // プログレスタイマー設置
    ProgressTimer* progressTimer { ProgressTimer::create(bar) };
    this->addChild(progressTimer);
    
    // 棒状
    progressTimer->setType(ProgressTimer::Type::BAR);
    
    progressTimer->setBarChangeRate(Vec2(1, 0));
    
    progressTimer->setMidpoint(Vec2(0, 0));
    
    this->progressTimer = progressTimer;
    
    return true;
}

// ％を設定
void StaminaBar::setPercentage(const float percentage)
{
    this->progressTimer->runAction(Sequence::createWithTwoActions(ProgressTo::create(0.05f, percentage), CallFunc::create([this, percentage]{this->changeColor(percentage);})));
}

// 色を変更
void StaminaBar::changeColor(const float percentage)
{
    if(percentage < WARNING_COLOR_CHANGE_THRESHOLD)
    {
        this->progressTimer->setColor(WARNING_COLOR);
    }
    else
    {
        this->progressTimer->setColor(NORMAL_COLOR);
    }
}

// 表示
void StaminaBar::slideIn(AnimationCallback cb)
{
    this->stopAllActions();
    this->runAction(Spawn::createWithTwoActions(EaseElasticOut::create(MoveTo::create(this->slideTime, this->inPosition), 0.5f), FadeIn::create(this->slideTime)));
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(this->slideTime), CallFunc::create([this, cb]{if(cb) cb(this);})));
}

// 非表示
void StaminaBar::slideOut(AnimationCallback cb)
{
    this->stopAllActions();
    this->runAction(Spawn::createWithTwoActions(EaseCubicActionIn::create(MoveTo::create(this->slideTime, this->outPosition)), FadeOut::create(this->slideTime)));
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(this->slideTime), CallFunc::create([this, cb]{if(cb) cb(this);})));
}
