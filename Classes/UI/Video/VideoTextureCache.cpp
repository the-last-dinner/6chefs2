
#include "VideoTextureCache.h"
#include "VideoDecode.h"

static pthread_mutex_t      s_asyncVideoPicQueueMutex;

static queue<VideoPic*>* s_pAsyncVideoPicQueue = NULL;

static VideoTextureCache *g_sharedTextureCache = NULL;

static pthread_t s_decodeThread;

static void *videoDecode(void *data)
{
    VideoDecode *p = (VideoDecode *) data;
    if(p) {
        while(p->decode()) {
            //sleep ?
        }
    }
    return 0;
}

VideoTextureCache * VideoTextureCache::sharedTextureCache()
{
    if (!g_sharedTextureCache) {
        g_sharedTextureCache = new VideoTextureCache();
    }
    return g_sharedTextureCache;
}

void VideoTextureCache::purgeSharedTextureCache()
{
    CC_SAFE_RELEASE_NULL(g_sharedTextureCache);
}

VideoTextureCache::VideoTextureCache()
{
    FUNCLOG;
    CCAssert(g_sharedTextureCache == NULL, "Attempted to allocate a second instance of a singleton.");
    m_pTextures = new Map<std::string, Ref *>();
    m_pVideoDecodes = new Map<std::string, Ref *>();
}

VideoTextureCache::~VideoTextureCache()
{
    FUNCLOG;
    pthread_mutex_destroy(&s_asyncVideoPicQueueMutex);
}

VideoDecode* VideoTextureCache::addVideo(const char *path)
{
    VideoDecode* pVideoDecode = (VideoDecode*)m_pVideoDecodes->at(path);

    if(!pVideoDecode) {
        pVideoDecode = new VideoDecode();
        if(pVideoDecode->init(path)) {
            m_pVideoDecodes->insert(path, pVideoDecode);
            pthread_create(&s_decodeThread, NULL, videoDecode, pVideoDecode);
            pVideoDecode->release();

            if (s_pAsyncVideoPicQueue == NULL) {
                
                s_pAsyncVideoPicQueue = new queue<VideoPic*>();
                pthread_mutex_init(&s_asyncVideoPicQueueMutex, NULL);
                
                Director::getInstance()->getScheduler()->schedule(schedule_selector(VideoTextureCache::picToTexture), this, 0, false);
            }
        } else {
            CCLOGERROR("CCVideoDecode init error in CCVideoTextureCache");
            return NULL;
        }
    } else {
        pVideoDecode->retain();
    }

    return pVideoDecode;
}

void VideoTextureCache::addPicData(VideoPic *pVideoPic)
{
    pthread_mutex_lock(&s_asyncVideoPicQueueMutex);
    s_pAsyncVideoPicQueue->push(pVideoPic);
    pthread_mutex_unlock(&s_asyncVideoPicQueueMutex);
}

void VideoTextureCache::picToTexture(float fd)
{
    VideoPic *pVideoPic = NULL;
    int length = m_pVideoDecodes->size();
    for(int i = 0; i < length; i++) {
        pthread_mutex_lock(&s_asyncVideoPicQueueMutex);
        if (!s_pAsyncVideoPicQueue->empty()) {
            pVideoPic = s_pAsyncVideoPicQueue->front();
            s_pAsyncVideoPicQueue->pop();
            pthread_mutex_unlock(&s_asyncVideoPicQueueMutex);
            if(pVideoPic) {
                addImageWidthData(pVideoPic->m_path, pVideoPic->m_frame,
                                  pVideoPic->m_pPicture,pVideoPic->m_length,
                                  Texture2D::PixelFormat::RGBA8888,
                                  pVideoPic->m_width, pVideoPic->m_height,
                                  Size(pVideoPic->m_width, pVideoPic->m_height)
                                  );
                pVideoPic->release();
            }
        } else {
            pthread_mutex_unlock(&s_asyncVideoPicQueueMutex);
            break;
        }
    }
}

void VideoTextureCache::removeVideo(const char *path)
{
    VideoDecode* pVideoDecode = (VideoDecode*)m_pVideoDecodes->at(path);
    if(pVideoDecode) {
        unsigned int rcount =  pVideoDecode->getReferenceCount();
        if(rcount == 1) {
            unsigned int frames = pVideoDecode->getFrames();
            for(; frames > 0; frames--) {
                removeTexture(path, frames);
            }
            m_pVideoDecodes->erase(path);
        } else {
            pVideoDecode->release();
        }
    }
}

Texture2D* VideoTextureCache::getTexture(const char *filename, int frame)
{
    ostringstream keystream;
    keystream << filename << "_" << frame;
    Texture2D * texture = NULL;
    texture = (Texture2D*)m_pTextures->at(keystream.str());
	return texture;
}

Texture2D* VideoTextureCache::addImageWidthData(const char *filename, int frame, const void *data, ssize_t dataLen, Texture2D::PixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const Size& contentSize)
{
    ostringstream keystream;
    keystream << filename << "_" << frame;

    string key = keystream.str();
    Texture2D * texture = NULL;
    texture = (Texture2D*)m_pTextures->at(key);
	if(!texture) {
        texture = new Texture2D();
        if( texture && 
        	texture->initWithData(data, dataLen, pixelFormat, pixelsWide, pixelsHigh, contentSize) ) {
            m_pTextures->insert(key, texture);
            texture->release();
		} else {
            CCLOG("cocos2d: Couldn't create texture for file:%s in CCVideoTextureCache", key.c_str());
        }
	} else {
        CCLOG("追加画像データ - %s", key.c_str());
    }

	return texture;
}

void VideoTextureCache::removeAllTextures()
{
    m_pTextures->clear();
}

void VideoTextureCache::removeTexture(const char *filename, int frame)
{
    ostringstream keystream;
    keystream << filename << "_" << frame;
    m_pTextures->erase(keystream.str());
}
