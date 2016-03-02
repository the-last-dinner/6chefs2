//
//  DisplayImageLayer.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/09.
//
//

#ifndef __LastSupper__DisplayImageLayer__
#define __LastSupper__DisplayImageLayer__

#include "Common.h"

class DisplayImageLayer : public Layer
{
// 定数
private:
    static const string DISP_IMG_PATH;
    static const float SHOW_DURATION;
    static const float HIDE_DURATION;
    
// クラスメソッド
public:
    static DisplayImageLayer* create(const string& imgFileName, const float duration, function<void()> callback);
    
// インスタンス変数
private:
    DisplayImageLayer();
    ~DisplayImageLayer();
    bool init(const string& imgFileName, const float duration, function<void()> callback);
    
    void remove(function<void()> callback);
};

#endif /* defined(__LastSupper__DisplayImageLayer__) */
