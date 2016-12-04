//
//  CountDownDisplay.h
//  6chefs2
//
//  Created by Ryoya Ino on 2016/12/03.
//
//

#ifndef CountDownDisplay_h
#define CountDownDisplay_h

#include "UI/SlideNode.h"

class CountDownDisplay : public SlideNode
{
    // 定数
private:
    static const float HORIZONTAL_MARGIN;
    static const float VERTICAL_MARGIN;
    static const float WARNING_COLOR_CHANGE_THRESHOLD;
    static const Color3B NORMAL_COLOR;
    static const Color3B WARNING_COLOR;
    
    // クラスメソッド
public:
    CREATE_FUNC(CountDownDisplay)
    
    // インスタンス変数
private:
    Label* _secondsLabel { nullptr };
    bool _isWarning { false };
    
    // インスタンスメソッド
private:
    CountDownDisplay();
    ~CountDownDisplay();
    bool init() override;
    void setWarningScaleAnimation();
    
public:
    void setSecondsLeft(const float& secondsLeft);
    void changeColor(const float percentage);
    virtual void slideIn(AnimationCallback cb = nullptr) override;
    virtual void slideOut(AnimationCallback cb = nullptr) override;
};

#endif /* CountDown_hpp */
