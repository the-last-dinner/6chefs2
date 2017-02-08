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

class VideoSprite : public Sprite
{
// 定数
private:
    static const int SDL_AUDIO_BUFFER_SIZE;
    static const int MAX_AUDIO_FRAME_SIZE;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(VideoSprite, const string&)
    
// インスタンス変数
private:
    Texture2D* _currentTexture { nullptr };
    bool _isPlaying { false };
    function<void(VideoSprite*)> _playFinishCallback { nullptr };
    
// インスタンスメソッド
private:
    VideoSprite();
    virtual ~VideoSprite();
    virtual bool init(const string& filename);
    void audioCallback(void *userdata, uint8_t* stream, int len);
public:
    void play();
};


#endif /* VideoSprite_h */
