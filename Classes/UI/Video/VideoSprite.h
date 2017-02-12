
#ifndef VideoSprite_h
#define VideoSprite_h

#include "define.h"

class VideoSprite : public Sprite
{  
public:
    static VideoSprite* create(const char* path);
    VideoSprite();
    virtual ~VideoSprite();
      
    bool init(const char* path);  
    void playVideo(void);  
    void stopVideo(void); 
    void seek(int frame);
    void update(float dt);  
      
    void registerPlayScriptHandler(int nHandler);
    void setVideoEndCallback(function<void()> func);
    
private:
    void unregisterPlayScriptHandler(void);
    unsigned int m_width;  
    unsigned int m_height;
    unsigned int m_frames;
    double m_frameRate;

    int m_playEndScriptHandler;
    unsigned int m_frame_count;
    string m_strFileName;
    bool m_enableTouchEnd;
    function<void()> m_videoEndCallback;
};  

#endif /* VideoSprite_h */
