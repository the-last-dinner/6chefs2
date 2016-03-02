//
//  StopWatch.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/27.
//
//

#include "Models/StopWatch.h"
#include "Event/GameEvent.h"
#include "Event/SceneEvent.h"

#pragma mark -
#pragma mark Init

// 初期化
bool StopWatch::init(const double& init_time)
{
    // 初期時間のセット
    this->time = init_time;
    return true;
}

bool StopWatch::init(const int init_time)
{
    return this->init(static_cast<double>(init_time));
}

#pragma mark -
#pragma mark MeasuringTime

// 時間計測開始
void StopWatch::tic()
{
    this->isCounting = true;
    this->start_time = this->getSecMs();
}

// 時間計測停止
void StopWatch::toc()
{
    double start = this->start_time;
    double stop = this->getSecMs();
    double interval_time = stop - start;
    this->time += interval_time;
    this->isCounting = false;
}

#pragma mark -
#pragma mark GetTime

// 時間取得
double StopWatch::getSecMs(){
    timeval tv;
    gettimeofday(&tv, nullptr);
    return (tv.tv_sec) + (tv.tv_usec) * 1e-6;
}

// 経過時間を取得
double StopWatch::getTime()
{
    if (this->isCounting)
    {
        this->toc();
        this->tic();
    }
    return this->time;
}

// 経過時間を秒で取得
int StopWatch::getTimeInt()
{
    return floor(this->getTime());
}

#pragma mark -
#pragma mark CountDown

// カウントダウン開始
void StopWatch::startCountDown(const float& interval_time)
{
    this->interval_time = interval_time;
    Director::getInstance()->getScheduler()->schedule(schedule_selector(StopWatch::scheduleFunction), this, this->interval_time, false);
    this->tic();
}

void StopWatch::startCountDown()
{
    this->startCountDown(this->interval_time);
}

// カウントダウン停止処理
void StopWatch::stopCountDown()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(StopWatch::scheduleFunction), this);
}

// スケジュール関数
void StopWatch::scheduleFunction(float delta)
{
    bool continueSchedule = true;
    
    // コールバック関数の実行
    if(this->scheduleCallback)
    {
        // 引数に経過時間、戻り値がTRUE
        continueSchedule = this->scheduleCallback(this->getTime());
    }
    
    // カウントダウンをストップ
    if (!continueSchedule)
    {
        this->stopCountDown();
    }
}

// カウントダウンイベントの保持
void StopWatch::setCountDown(GameEvent *event)
{
    this->countDownEvent = event;
    CC_SAFE_RETAIN(this->countDownEvent);
}