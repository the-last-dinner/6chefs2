//
//  StaminaBar.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/30.
//
//

#ifndef StaminaBar_h
#define StaminaBar_h

#include "UI/SlideNode.h"

class StaminaBar : public SlideNode
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
    CREATE_FUNC(StaminaBar)

// インスタンス変数
private:
    ProgressTimer* progressTimer { nullptr };
    
// インスタンスメソッド
private:
    StaminaBar();
    ~StaminaBar();
    bool init();

public:
    void setPercentage(const float percentage);
    void changeColor(const float percentage);
    virtual void slideIn(AnimationCallback cb = nullptr) override;
    virtual void slideOut(AnimationCallback cb = nullptr) override;
};

#endif /* ProgressBar_h */
