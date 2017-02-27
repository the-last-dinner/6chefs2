#include "VideoDecode.h"
#include "VideoTextureCache.h"

#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "swresample.lib")

VideoPic::VideoPic()
{
    FUNCLOG;
    m_pPicture = NULL;
}

VideoPic::~VideoPic()
{
    FUNCLOG;
    if(m_pPicture) {
        delete m_pPicture; 
    }
}


bool VideoPic::init(const char *path, int frame,unsigned int width,  unsigned int height, unsigned char* data)
{
    m_width = width;
    m_height = height;
    m_frame = frame;
    m_path = path;
    m_length = m_width * m_height * 4;
    m_pPicture = new unsigned char[m_length];
    for(unsigned int i = 0; i < m_length; ++i) {
        m_pPicture[i] = data[i];
    }
    return true;
}

VideoDecode::VideoDecode()
{
    FUNCLOG;
	m_filepath = NULL;
	m_pFormatCtx = NULL;
	m_videoStream = -1;
    m_pSwsCtx = NULL;
    m_pCodecCtx = NULL;
    m_frameCount = 0;
}

bool VideoDecode::init(const char *path)
{
    m_filepath = path;
    // ライブラリ初期化
    av_register_all();  
    
    // ファイルを開く
    if(avformat_open_input(&m_pFormatCtx, path, NULL, NULL) != 0) {  
         CCLOG("avformat_open_input false ファイルが開けないよ");
         return false;  
    }
    
    // ストリームの情報を得る
    if(avformat_find_stream_info(m_pFormatCtx, NULL) < 0) {  
        CCLOG("avformat_find_stream_info false 情報が見つからないよ");
        return false;  
    } 
    
    // 最初の映像ストリームを探す
    m_videoStream = -1;
   
    for(int i=0; i < m_pFormatCtx->nb_streams; i++) {  
        
        if(m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_videoStream = i; 
            break;
        }
    } 
    
    // 映像ストリームが見つからない
    if(m_videoStream == -1) {
        CCLOGERROR("映像ストリームが見つからないよ");  
        return false;  
    } 

    // 映像ストリームのコーデックコンテキストのポインターを取得する
    m_pCodecCtx = m_pFormatCtx->streams[m_videoStream]->codec;
    // サイズを取得
    if (m_pCodecCtx->width) {
        m_width = m_pCodecCtx->width;
        m_height = m_pCodecCtx->height;
    } else {
        CCLOGERROR("サイズが取得できないよ");
        return false;
    }

    
    // 長さ
    int64_t duration = 0;
    if (m_pFormatCtx->duration != AV_NOPTS_VALUE) {
        duration = m_pFormatCtx->duration + 5000;
    } else {
        CCLOGERROR("長さがわからないよ");
        return false;
    }
    
    // fps
    AVRational rational;

    if(m_pFormatCtx->streams[m_videoStream]->avg_frame_rate.den && m_pFormatCtx->streams[m_videoStream]->avg_frame_rate.num) {
        rational = m_pFormatCtx->streams[m_videoStream]->avg_frame_rate;
    } else if(m_pFormatCtx->streams[m_videoStream]->r_frame_rate.den && m_pFormatCtx->streams[m_videoStream]->r_frame_rate.num) {
        rational = m_pFormatCtx->streams[m_videoStream]->r_frame_rate;
    } else {
        CCLOGERROR("fpsが取得できないよ");
        return false;
    }
    double fps = av_q2d(rational);
    m_frameRate = 1.0 / fps;
    m_frames = (int)((fps * duration) / AV_TIME_BASE) - 1;
    CCLOG("m_frameRate = %f , frames = %d", m_frameRate, m_frames);



    AVCodec *pCodec = NULL;
    // 映像ストリームに適したコーデックを探す
    pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);

    if(pCodec == NULL) { 
        CCLOGERROR("avcodec_find_decoder error 対応していないコーデックだよ");
        return false;
    }  
    
    // コーデックを開く
    if(avcodec_open2(m_pCodecCtx, pCodec, NULL) < 0) {
        CCLOGERROR("avcodec_open2 error コーデックが開けないよ");
        return false;
    }  

    return true;
}

unsigned int VideoDecode::getWidth()
{
    return m_width;
}

unsigned int VideoDecode::getHeight()
{
    return m_height;
}

double VideoDecode::getFrameRate()
{
    return m_frameRate;
}

unsigned int VideoDecode::getFrames()
{
    return m_frames;
}

const char* VideoDecode::getFilePath()
{
    return m_filepath;
}

VideoDecode::~VideoDecode()
{
    FUNCLOG;
    if (m_pCodecCtx) avcodec_close(m_pCodecCtx);  
    if (m_pFormatCtx) avformat_close_input(&m_pFormatCtx);

}

// デコードしてcocosで表示できる形式の画像に分解する
bool VideoDecode::decode()
{
    if(m_frameCount == -1)
        return false;

    AVPacket packet;
    int frameFinished = 0;
    m_pFrame = NULL;
    while(!frameFinished && av_read_frame(m_pFormatCtx, &packet) >= 0) {
        if(packet.stream_index== m_videoStream) {
            // 映像フレームにメモリーを割り当てる
            m_pFrame = av_frame_alloc();

            int lentmp = avcodec_decode_video2(m_pCodecCtx, m_pFrame, &frameFinished, &packet);
            if(lentmp <= 0) {
                av_free(m_pFrame);
                return false;
            }
        }
        av_packet_unref(&packet);
    }

    if(m_pFrame == NULL) {
        return false;
    }
    
    // 変換の設定
    m_pSwsCtx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt, m_width, m_height, AV_PIX_FMT_RGBA, SWS_SPLINE, NULL, NULL, NULL);
    if(!m_pSwsCtx) {
        CCLOGERROR("sws_getContext error");
        return false;
    }

    AVFrame *m_pFrameRGB;
    // 変換後の映像フレームにメモリーを割り当てる
    m_pFrameRGB = av_frame_alloc();
    m_pFrameRGB->width = m_width;
    m_pFrameRGB->height = m_height;
    av_image_alloc(m_pFrameRGB->data, m_pFrameRGB->linesize, m_width, m_height, AV_PIX_FMT_RGBA, 32);
    
    CCLOG("avpicture_alloc width = %d height = %d m_videoStream = %d", m_width, m_height, m_videoStream);
    // 変換する
    sws_scale(m_pSwsCtx, m_pFrame->data, m_pFrame->linesize, 0, m_height, m_pFrameRGB->data, m_pFrameRGB->linesize);
    
    m_frameCount++;

    // 変換した画像をセット
    VideoPic *pVideoPic = new VideoPic();
    pVideoPic->init(m_filepath, m_frameCount, m_width, m_height, m_pFrameRGB->data[m_videoStream]);
    VideoTextureCache::sharedTextureCache()->addPicData(pVideoPic);

    if (frameFinished == 0) {
        m_frameCount = -1;
    }
    
    // メモリ解放
    av_freep(m_pFrameRGB);
    av_free(m_pFrame);
    av_packet_unref(&packet);
    av_freep(&packet);
    sws_freeContext(m_pSwsCtx);
    return true;
}

