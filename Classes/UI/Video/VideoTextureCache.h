
#ifndef VideoTextureCache_h
#define VideoTextureCache_h

#include "define.h"

#include "VideoDecode.h"

class VideoTextureCache : public Ref
{
	protected:
        Map<string, Ref *>* m_pTextures;
        Map<string, Ref *>* m_pVideoDecodes;
        bool _threadEnd = false;
        string _delKey = "";
	public:
        VideoTextureCache();
        virtual ~VideoTextureCache();

        static VideoTextureCache * sharedTextureCache();
		static void purgeSharedTextureCache();
		
        Texture2D* addImageWidthData(const char *filename, int frame, const void *data, ssize_t dataLen, Texture2D::PixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const Size& contentSize);
    
        Texture2D* getTexture(const char *filename, int frame);

	    void removeAllTextures();

	    void removeTexture(const char *filename, int frame);

        VideoDecode* addVideo(const char *path);
	    void removeVideo(const char *path);

        void addPicData(VideoPic *pVideoPic);

        void picToTexture(float fd);

};

#endif /* VideoTextureCache_h */
