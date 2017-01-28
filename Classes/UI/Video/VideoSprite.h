//
//  VideoSprite.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/06/05.
//
//

#ifndef VideoSprite_h
#define VideoSprite_h

#include "define.h"

namespace cv {
    class VideoCapture;
}
class AVCodecContext;
class AVFormatContext;
class SDL_Mutex;
class SDL_cond;

class VideoSprite : public Sprite
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(VideoSprite, const string&)
    
// インスタンス変数
private:
    // visual
    cv::VideoCapture* _videoCapture { nullptr };
    Texture2D* _currentTexture { nullptr };
    
    // sound
    
    bool _isPlaying { false };
    function<void(VideoSprite*)> _playFinishCallback { nullptr };
    
// インスタンスメソッド
private:
    VideoSprite();
    virtual ~VideoSprite();
    virtual bool init(const string& filename);
public:
    void play();
    void update(float delta) override;
};


#endif /* VideoSprite_h */
