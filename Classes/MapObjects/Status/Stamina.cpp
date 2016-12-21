//
//  Stamina.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/29.
//
//

#include "MapObjects/Status/Stamina.h"

// 定数
const float Stamina::MAX_VALUE { 100.f };
const float Stamina::MIN_VALUE { 0.f };
const float Stamina::DEFAULT_STEP {1.0f};
const float Stamina::INCREASE_STEP_RATIO {0.5f};
const float Stamina::UPDATE_INTERVAL {0.05f};
const float Stamina::WARN_PERCENTAGE_THRESHOLD { 33.f };

// コンストラクタ
Stamina::Stamina() {FUNCLOG};

// デストラクタ
Stamina::~Stamina()
{
    FUNCLOG
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
};

// 初期化
bool Stamina::init()
{
    Director::getInstance()->getScheduler()->schedule(CC_SCHEDULE_SELECTOR(Stamina::update), this, UPDATE_INTERVAL, false);
    
    return true;
}

// スタミナ回復
void Stamina::increase()
{
    float percentage { _percentage };
    
    percentage += DEFAULT_STEP * INCREASE_STEP_RATIO;
    
    // 最大値以上になっていたら、最大値にして、歳立ちになったら疲労状態解除
    if (percentage > MAX_VALUE) {
        percentage = MAX_VALUE;
        _exhausted = false;
    }
    
    // メンバ変数にセット
    this->setPercentage(percentage);
}

// スタミナ減少
void Stamina::decrease()
{
    this->decrease(DEFAULT_STEP * _ratio);
}

// スタミナ減少
void Stamina::decrease(float value)
{
    float percentage { _percentage };
    
    percentage -= value;
    if (percentage < MIN_VALUE) {
        percentage = MIN_VALUE;
        _exhausted = true;
    }
    
    this->setPercentage(percentage);
}

// スタミナを設定
void Stamina::setPercentage(const float percentage)
{
    _percentage = percentage;
    
    if(this->onPercentageChanged) this->onPercentageChanged(percentage);
    if(this->isMax() && this->onIncreasedMax) this->onIncreasedMax();
}

// 減少状態へ移行
void Stamina::setDecreasing(const bool decreasing)
{
    // 減少中状態へ
    _decreasing = decreasing;
}

// ％を取得
float Stamina::getPercentage() const
{
    return _percentage;
}

// 停止状態フラグを設定
void Stamina::setPaused(bool paused)
{
    _paused = paused;
}

// スタミナ減少の速度倍率を設定
void Stamina::setStepRatio(const float ratio)
{
    _ratio = ratio;
}

// 疲労状態かどうか
bool Stamina::isExhausted() const
{
    return _exhausted;
}

// スタミナがあるかどうか
bool Stamina::isEmpty() const
{
    return _percentage == MIN_VALUE;
}

// スタミナが危険状態か
bool Stamina::isWarn() const
{
    return _percentage < WARN_PERCENTAGE_THRESHOLD;
}

// スタミナが最大かどうか
bool Stamina::isMax() const
{
    return _percentage == MAX_VALUE;
}

// update
void Stamina::update(float delta)
{
    if (_paused) return;
    
    // 減少状態と、回復状態で場合分け
    if (_decreasing) {
        this->decrease();
        if (this->isEmpty()) this->setDecreasing(false);
        return;
    }
    
    this->increase();
}
