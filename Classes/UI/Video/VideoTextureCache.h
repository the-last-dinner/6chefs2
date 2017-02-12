/**
 * 视频纹理缓存
 * @author leoluo<luochong1987@gmail.com>
 */
//#ifndef __CCVIDEOTEXTURE_CACHE_H__
//#define __CCVIDEOTEXTURE_CACHE_H__

#ifndef VideoTextureCache_h
#define VideoTextureCache_h

#include "define.h"

//#include "cocoa/CCObject.h"
//#include "cocoa/CCDictionary.h"
//#include "textures/CCTexture2D.h"
//#include <string>
//#include "CCVideoDecode.h"
#include "VideoDecode.h"
 
//NS_CC_BEGIN
//class CC_DLL CCVideoTextureCache : public CCObject
class CC_DLL VideoTextureCache : public Ref
{
	protected:
		//CCDictionary* m_pTextures;
		//CCDictionary* m_pVideoDecodes;
        Map<std::string, Ref *>* m_pTextures;
        Map<std::string, Ref *>* m_pVideoDecodes;
	public:
		//CCVideoTextureCache();
        //virtual ~CCVideoTextureCache();
        VideoTextureCache();
        virtual ~VideoTextureCache();

		//static CCVideoTextureCache * sharedTextureCache();
        static VideoTextureCache * sharedTextureCache();
		static void purgeSharedTextureCache();
		
		//CCTexture2D* addImageWidthData(const char *filename, int frame, const void *data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize);
        Texture2D* addImageWidthData(const char *filename, int frame, const void *data, ssize_t dataLen, Texture2D::PixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const Size& contentSize);
		//CCTexture2D* getTexture(const char *filename, int frame);
        Texture2D* getTexture(const char *filename, int frame);

	    void removeAllTextures();

	    void removeTexture(const char *filename, int frame);

	    /**
	     * 视频解码
	     */
	    //CCVideoDecode* addVideo(const char *path);
        VideoDecode* addVideo(const char *path);
	    void removeVideo(const char *path);

	    //void addPicData(CCVideoPic *pVideoPic);
        void addPicData(VideoPic *pVideoPic);
	    //void picToTexture();
        void picToTexture(float fd);

};
//NS_CC_END
//#endif //__CCVIDEOTEXTURE_CACHE_H__
#endif /* VideoTextureCache_h */
