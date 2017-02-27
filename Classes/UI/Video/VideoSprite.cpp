
#include "VideoSprite.h"
#include "VideoTextureCache.h"
#include "VideoDecode.h"

extern "C" { 
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"  
}  

VideoSprite* VideoSprite::create(const char* path)
{
    VideoSprite* video = new VideoSprite();

    if (video && video->init(path)) {  
         video->autorelease();
         return video;
    }
    CC_SAFE_DELETE(video);
    return NULL;  
}  

VideoSprite::VideoSprite()
{
    FUNCLOG;
    m_frameRate = 1.0 / 31;
    m_frame_count = 1;  
    m_enableTouchEnd = false;  
    m_width = 100;
    m_height = 100;
    m_playEndScriptHandler = 0;
}  

VideoSprite::~VideoSprite()
{  
    FUNCLOG;
    VideoTextureCache* video = VideoTextureCache::sharedTextureCache();
    video->removeVideo(m_strFileName.c_str());
    unregisterPlayScriptHandler();
    CC_SAFE_RELEASE_NULL(video);
}

bool VideoSprite::init(const char* path)
{  
    m_strFileName = path;

    VideoDecode *pVideoDecode = VideoTextureCache::sharedTextureCache()->addVideo(path);
    if(!pVideoDecode)
    {
        CCLOGERROR("videoDecode get error in %s", "VideoSprite");
        return false;
    }

    m_width = pVideoDecode->getWidth();
    m_height = pVideoDecode->getHeight();
    m_frames = pVideoDecode->getFrames();
    m_frameRate = pVideoDecode->getFrameRate();

    Texture2D *texture = new Texture2D();

    unsigned int length = m_width * m_height * 4;
    unsigned char* tempData = new unsigned char[length];
    for(unsigned int i = 0; i < length; ++i) {
        tempData[i] = 0;
    }

    texture->initWithData(tempData, length, Texture2D::PixelFormat::RGBA8888, m_width, m_height, Size(m_width, m_height));
    
    initWithTexture(texture);

    this->setContentSize(Size(m_width, m_height));

    delete [] tempData;

    return true;  
}

void VideoSprite::playVideo()
{  
    update(0);
    this->schedule(schedule_selector(VideoSprite::update), m_frameRate);
}  

void VideoSprite::stopVideo(void)
{
    this->unscheduleAllCallbacks();
    VideoTextureCache::sharedTextureCache()->removeAllTextures();
}  

void VideoSprite::seek(int frame)
{  
    m_frame_count = frame;
    update(0);
}  

void VideoSprite::update(float dt)
{

    Texture2D *texture = NULL;
    texture = VideoTextureCache::sharedTextureCache()->getTexture(m_strFileName.c_str(), m_frame_count);
    if(texture) {
        m_frame_count++;
        setTexture(texture);
        if(m_frame_count >= m_frames) {
            m_frame_count = 1;
            if (m_videoEndCallback)
                m_videoEndCallback();
            
            if(m_playEndScriptHandler)
                ScriptEngineManager::getInstance()->getScriptEngine()->reallocateScriptHandler(m_playEndScriptHandler);
        }  
    } else {
        CCLOG("表示失敗 VideoSprite::update filename = %s , now_frame = %d, total_frame = %d", m_strFileName.c_str(), m_frame_count, m_frames);
    }

}

void VideoSprite::registerPlayScriptHandler(int nHandler)
{
    unregisterPlayScriptHandler();
    m_playEndScriptHandler = nHandler;
    LUALOG("[LUA] Add VideoSprite event handler: %d", m_playEndScriptHandler);
}

void VideoSprite::unregisterPlayScriptHandler(void)
{
    if (m_playEndScriptHandler) {
        ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(m_playEndScriptHandler);
        LUALOG("[LUA] Remove VideoSprite event handler: %d", m_playEndScriptHandler);
        m_playEndScriptHandler = 0;
    }
}

void VideoSprite::setVideoEndCallback(function<void()> func)
{  
    m_videoEndCallback = func;  
}
