//
//  StopWatch.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/27.
//
//

#ifndef StopWatch_h
#define StopWatch_h

#include "define.h"

class GameEvent;

class StopWatch : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(StopWatch, const double&)
    CREATE_FUNC_WITH_PARAM(StopWatch, const int)
private:
    StopWatch(){FUNCLOG};
    ~StopWatch();

// インスタンス変数
private:
    // 時間計測開始時間(単位:ms)
    double startTime {0.0};
    // 計測時間
    double time {0.0};
    // インターバルタイム
    float intervalTime {0.0};
    // カウントダウン用
    GameEvent* countDownEvent {nullptr};
    // カウントしている状態か
    bool isCounting {false};

public:
    // スケジュール用コールバック関数
    function<bool(double)> scheduleCallback {nullptr};

// インスタンスメソッド
private:
    bool init(const double& initTime);
    bool init(const int initTime);
public:
    // 元の時間取得
    double getSecMs();
    // 時間取得
    double getTime();
    int getTimeInt();
    // 時間計測処理
    void tic();
    void toc();
    // スケジュール実行
    void startCountDown(const float& intervalTime);
    void startCountDown();
    void stopCountDown();
    void scheduleFunction(float delta);
    void setCountDownEvent(GameEvent* event);
};

#endif /* StopWatch_h */
