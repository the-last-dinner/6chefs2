//
//  ButtonMashingLayer.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/10.
//
//

#ifndef __LastSupper__ButtonMashingLayer__
#define __LastSupper__ButtonMashingLayer__

#include "Common.h"

class ButtonMashingLayer : public Layer
{
// 定数
public:
    enum struct Result
    {
        SUCCESS,
        FAILURE,
        
        SIZE,
    };
    
    using ResultCallback = function<void(Result)>;
    
// クラスメソッド
public:
    static ButtonMashingLayer* create(int time, float limit, string fileName, ResultCallback callback);
    
// インスタンス変数
private:
    int count { 0 };
    string fileName {};
    ResultCallback callback { nullptr };
    
// インスタンスメソッド
private:
    ButtonMashingLayer();
    ~ButtonMashingLayer();
    bool init(int time, float limit, string fileName, ResultCallback callback);
    
    void onEnterKeyPressed();
};

#endif /* defined(__LastSupper__ButtonMashingLayer__) */
