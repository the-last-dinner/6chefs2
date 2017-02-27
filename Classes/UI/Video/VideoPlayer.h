//
//  VideoPlayer.h
//  6chefs2
//
//  Created by Sota Inami on 2017/2/12.
//
//

#ifndef VideoPlayer_h
#define VideoPlayer_h

#include "Common.h"

#include "UI/Video/VideoSprite.h"

class VideoPlayer : public Layer
{
// 定数
private:
    static const string DISP_VIDEO_PATH;
    static const float SHOW_DURATION;
    static const float HIDE_DURATION;
    
// クラスメソッド
public:
    static VideoPlayer* create(const string& fileName, const bool skip, function<void()> onEnd);
    
private:
    VideoPlayer();
    ~VideoPlayer();
    bool init(const string& fileName, const bool skip, function<void()> onEnd);
    
    void remove(function<void()> callback);
};

#endif /* VideoPlayer_h */
