
#ifndef VideoLayer_h
#define VideoLayer_h

#include "define.h"

class VideoLayer : public Sprite
{  
public:
    static VideoLayer* create(const char* path);
    VideoLayer();
    virtual ~VideoLayer();
      
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

#endif /* VideoLayer_h */
