//
//  Stamina.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/29.
//
//

#ifndef Stamina_h
#define Stamina_h

#include "define.h"

class Stamina : public Ref
{
// 定数
public:
    static const float MAX_VALUE;
    static const float MIN_VALUE;
    static const float DEFAULT_STEP;
    static const float INCREASE_STEP_RATIO;
    static const float UPDATE_INTERVAL;
    static const float WARN_PERCENTAGE_THRESHOLD;
    
// クラスメソッド
public:
    CREATE_FUNC(Stamina)
    
// インスタンス変数
private:
    float _percentage { MAX_VALUE };
    float _ratio { 1.0f };
    bool _decreasing {false};
    bool _exhausted {false};
    bool _paused {false};
public:
    function<void(float)> onPercentageChanged { nullptr };
    function<void()> onIncreasedMax { nullptr };
    
// インスタンスメソッド
private:
    Stamina();
    ~Stamina();
    bool init();
    void increase();
    void decrease();
    void setPercentage(const float percentage);
public:
    void setPaused(bool paused);
    void setStepRatio(const float ratio);
    void setDecreasing(const bool decreasing);
    float getPercentage() const;
    bool isExhausted() const;
    bool isEmpty() const;
    bool isWarn() const;
    bool isMax() const;
    void decrease(float value);
    void update(float delta);
};

#endif /* Stamina_h */
