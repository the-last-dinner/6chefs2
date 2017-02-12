/**
 * 视频材质缓存类
 */

//#include "CCVideoTextureCache.h"
#include "VideoTextureCache.h"
//#include "CCVideoDecode.h"
#include "VideoDecode.h"
//#include "CCDirector.h"
//#include "CCScheduler.h"
//#include <sstream>
//#include <queue>
//#include "platform/platform.h"
//#include "platform/CCThread.h"

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
//#include <pthread.h>
//#else
//#include "CCPThreadWinRT.h"
//#include <ppl.h>
//#include <ppltasks.h>
//using namespace concurrency;
// #endif


//using namespace std;
//NS_CC_BEGIN

static pthread_mutex_t      s_asyncVideoPicQueueMutex;
//static std::queue<CCVideoPic*>* s_pAsyncVideoPicQueue = NULL;
static queue<VideoPic*>* s_pAsyncVideoPicQueue = NULL;


static VideoTextureCache *g_sharedTextureCache = NULL;

static pthread_t s_decodeThread;


static void *videoDecode(void *data)
{
    //CCVideoDecode *p = (CCVideoDecode *) data;
    VideoDecode *p = (VideoDecode *) data;
    if(p)
    {
        while(p->decode())
        {
            //sleep ?
        }
    }
    return 0;
}



//CCVideoTextureCache * CCVideoTextureCache::sharedTextureCache()
VideoTextureCache * VideoTextureCache::sharedTextureCache()
{
    if (!g_sharedTextureCache)
    {
        g_sharedTextureCache = new VideoTextureCache();
    }
    return g_sharedTextureCache;
}

//void CCVideoTextureCache::purgeSharedTextureCache()
void VideoTextureCache::purgeSharedTextureCache()
{
    CC_SAFE_RELEASE_NULL(g_sharedTextureCache);
}


//CCVideoTextureCache::CCVideoTextureCache()
VideoTextureCache::VideoTextureCache()
{
    CCAssert(g_sharedTextureCache == NULL, "Attempted to allocate a second instance of a singleton.");
    m_pTextures = new Map<std::string, Ref *>();
    m_pVideoDecodes = new Map<std::string, Ref *>();
}

//CCVideoTextureCache::~CCVideoTextureCache()
VideoTextureCache::~VideoTextureCache()
{
    CCLOGINFO("cocos2d: deallocing CCVideoTextureCache.");
    //CC_SAFE_RELEASE(m_pTextures);
    //CC_SAFE_RELEASE(m_pVideoDecodes);
    pthread_mutex_destroy(&s_asyncVideoPicQueueMutex);
}


//CCVideoDecode* CCVideoTextureCache::addVideo(const char *path)
VideoDecode* VideoTextureCache::addVideo(const char *path)
{

    //CCVideoDecode* pVideoDecode = (CCVideoDecode*)m_pVideoDecodes->objectForKey(path);
    //VideoDecode* pVideoDecode = (VideoDecode*)m_pVideoDecodes->objectForKey(path);
    VideoDecode* pVideoDecode = (VideoDecode*)m_pVideoDecodes->at(path);

    if(!pVideoDecode)
    {
        //pVideoDecode = new CCVideoDecode();
        pVideoDecode = new VideoDecode();
        if(pVideoDecode->init(path))
        {
            //m_pVideoDecodes->setObject(pVideoDecode, path);
            m_pVideoDecodes->insert(path, pVideoDecode);
            //开启线程进行解码
            pthread_create(&s_decodeThread, NULL, videoDecode, pVideoDecode);
            pVideoDecode->release();


            if (s_pAsyncVideoPicQueue == NULL)
            {   
                //s_pAsyncVideoPicQueue = new queue<CCVideoPic*>();
                s_pAsyncVideoPicQueue = new queue<VideoPic*>();
                pthread_mutex_init(&s_asyncVideoPicQueueMutex, NULL);
                //CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CCVideoTextureCache::picToTexture), this, 0, false);
                //CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(VideoTextureCache::picToTexture), this, 0, false);
                Director::getInstance()->getScheduler()->schedule(schedule_selector(VideoTextureCache::picToTexture), this, 0, false);
            }
        }
        else
        {
            CCLOGERROR("CCVideoDecode init error in CCVideoTextureCache");
            return NULL;
        }
    }
    else
    {
        pVideoDecode->retain();
    }

    return pVideoDecode;
}


//void CCVideoTextureCache::addPicData(CCVideoPic *pVideoPic)
void VideoTextureCache::addPicData(VideoPic *pVideoPic)
{
    pthread_mutex_lock(&s_asyncVideoPicQueueMutex);
    s_pAsyncVideoPicQueue->push(pVideoPic);
    pthread_mutex_unlock(&s_asyncVideoPicQueueMutex);
}

/**
 *  图片转纹理
 */
//void CCVideoTextureCache::picToTexture()
void VideoTextureCache::picToTexture(float fd)
{
    //CCVideoPic *pVideoPic = NULL;
    VideoPic *pVideoPic = NULL;
    //int length = m_pVideoDecodes->count();
    int length = m_pVideoDecodes->size();
    for(int i = 0; i < length; i++)
    {
        pthread_mutex_lock(&s_asyncVideoPicQueueMutex);
        if (!s_pAsyncVideoPicQueue->empty())
        {
            pVideoPic = s_pAsyncVideoPicQueue->front();
            s_pAsyncVideoPicQueue->pop();
            pthread_mutex_unlock(&s_asyncVideoPicQueueMutex);
            if(pVideoPic)
            {
                
                // Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRGGGGBBBBAAAA"
                /*  我还会回来的
                unsigned int length = pVideoPic->m_width * pVideoPic->m_height;
                unsigned int* inPixel32 = (unsigned int*)pVideoPic->m_pPicture->data[pVideoPic->m_videoStream];  
                unsigned char* tempData = new unsigned char[pVideoPic->m_width * pVideoPic->m_height * 2];
                unsigned short* outPixel16 = (unsigned short*)tempData;
                
                for(unsigned int i = 0; i < length; ++i, ++inPixel32)
                {
                    *outPixel16++ = 
                    ((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | // R
                    ((((*inPixel32 >> 8) & 0xFF) >> 4) <<  8) | // G
                    ((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | // B
                    ((((*inPixel32 >> 24) & 0xFF) >> 4) << 0);  // A
                }
                */
                //addImageWidthData(pVideoPic->m_path, pVideoPic->m_frame,pVideoPic->m_pPicture,kCCTexture2DPixelFormat_RGBA8888,pVideoPic->m_width, pVideoPic->m_height,CCSize(pVideoPic->m_width, pVideoPic->m_height));
                addImageWidthData(pVideoPic->m_path, pVideoPic->m_frame,
                                  pVideoPic->m_pPicture,pVideoPic->m_length,
                                  Texture2D::PixelFormat::RGBA8888,
                                  pVideoPic->m_width, pVideoPic->m_height,
                                  Size(pVideoPic->m_width, pVideoPic->m_height)
                                  );

               // delete [] tempData;
                pVideoPic->release();
            }
        }
        else
        {
            pthread_mutex_unlock(&s_asyncVideoPicQueueMutex);
            break;
        }
    }
}

//void CCVideoTextureCache::removeVideo(const char *path)
void VideoTextureCache::removeVideo(const char *path)
{
    //CCVideoDecode* pVideoDecode = (CCVideoDecode*)m_pVideoDecodes->objectForKey(path);
    //VideoDecode* pVideoDecode = (VideoDecode*)m_pVideoDecodes->objectForKey(path);
    VideoDecode* pVideoDecode = (VideoDecode*)m_pVideoDecodes->at(path);
    if(pVideoDecode)
    {
        unsigned int rcount =  pVideoDecode->getReferenceCount();
        if(rcount == 1)
        {
            unsigned int frames = pVideoDecode->getFrames();
            for(; frames > 0; frames--)
            {
                removeTexture(path, frames);
            }
            //m_pVideoDecodes->removeObjectForKey(path);
            m_pVideoDecodes->erase(path);
        }
        else
        {
            pVideoDecode->release();
        }
    }
}


//CCTexture2D* CCVideoTextureCache::getTexture(const char *filename, int frame)
Texture2D* VideoTextureCache::getTexture(const char *filename, int frame)
{
    std::ostringstream keystream;
    keystream << filename << "_" << frame;
    //CCTexture2D * texture = NULL;
    Texture2D * texture = NULL;
	//texture = (CCTexture2D*)m_pTextures->objectForKey(keystream.str());
    texture = (Texture2D*)m_pTextures->at(keystream.str());
	return texture;
}
    	
//CCTexture2D* CCVideoTextureCache::addImageWidthData(const char *filename, int frame, const void *data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize)
Texture2D* VideoTextureCache::addImageWidthData(const char *filename, int frame, const void *data, ssize_t dataLen, Texture2D::PixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const Size& contentSize)
{
    std::ostringstream keystream;
    keystream << filename << "_" << frame;

    //CCLOG("cocos2d: create texture for file:%s in CCVideoTextureCache", keystream.str().c_str());
    std::string key = keystream.str();
    //CCTexture2D * texture = NULL;
    Texture2D * texture = NULL;
	//texture = (CCTexture2D*)m_pTextures->objectForKey(key);
    //texture = (Texture2D*)m_pTextures->objectForKey(key);
    texture = (Texture2D*)m_pTextures->at(key);
	if(!texture)
	{
		//texture = new CCTexture2D();
        texture = new Texture2D();
        if( texture && 
        	texture->initWithData(data, dataLen, pixelFormat, pixelsWide, pixelsHigh, contentSize) )
        {
            //CCLOG("cocos2d: create texture for file:%s in CCVideoTextureCache", key.c_str());
            //m_pTextures->setObject(texture, key);
            m_pTextures->insert(key, texture);
            texture->release();
		}
		else
        {
            CCLOG("cocos2d: Couldn't create texture for file:%s in CCVideoTextureCache", key.c_str());
        }
	}
    else
    {
        CCLOG("纹理已经存在 - %s", key.c_str());
    }

	return texture;
}

//void CCVideoTextureCache::removeAllTextures()
void VideoTextureCache::removeAllTextures()
{
    //m_pTextures->removeAllObjects();
    m_pTextures->clear();
}


/** 
 * Deletes a texture from the cache given a its key name
 */
//void CCVideoTextureCache::removeTexture(const char *filename, int frame)
void VideoTextureCache::removeTexture(const char *filename, int frame)
{
    std::ostringstream keystream;
    keystream << filename << "_" << frame;
    //m_pTextures->removeObjectForKey(keystream.str());
    m_pTextures->erase(keystream.str());
}
//NS_CC_END


