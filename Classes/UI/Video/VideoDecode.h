#ifndef VideoDecode_h
#define VideoDecode_h

#include "define.h"

extern "C" { 
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"  
    #include "libswscale/swscale.h"
    #include "libavutil/imgutils.h"
}

struct AVFormatContext;  
struct AVCodecContext;  
struct AVFrame;  
struct AVPicture;  
struct SwsContext; 

// 動画の１枚毎の画像
class VideoPic : public Ref
{
	public:
        VideoPic();
		bool init(const char *path, int frame,unsigned int width,  unsigned int height, unsigned char* data);
        virtual ~VideoPic();
		const char *m_path;
		int m_frame;
		int m_width;
		int m_height;
		unsigned char* m_pPicture;
        unsigned int m_length;
};

// 動画を画像に変換
class VideoDecode : public Ref
{
	private:
		AVFormatContext *m_pFormatCtx;  
	    AVCodecContext *m_pCodecCtx;  
	    AVFrame *m_pFrame;  
	    
	    int m_videoStream;
	    SwsContext *m_pSwsCtx;

	    int m_frameCount;
	    const char *m_filepath;
	    double m_frameRate;
	    unsigned int m_frames;
	    unsigned int m_width;  
    	unsigned int m_height;   
	public:
        VideoDecode();
        virtual ~VideoDecode();
		bool decode();
		bool init(const char *path);
		unsigned int getWidth();
		unsigned int getHeight();
		double getFrameRate();
		unsigned int getFrames();
		const char* getFilePath(); 
};

#endif /* VideoDecode_h */
