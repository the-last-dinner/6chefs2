//
//  VideoSprite.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/06/05.
//
//

#include "UI/Video/VideoSprite.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h"
#include "libavfilter/avfilter.h"
#include <libavresample/avresample.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
}

// 定数
const int VideoSprite::SDL_AUDIO_BUFFER_SIZE { 1024 };
const int VideoSprite::MAX_AUDIO_FRAME_SIZE { 192000 };

// コンストラクタ
VideoSprite::VideoSprite() { FUNCLOG }

// デストラクタ
VideoSprite::~VideoSprite() { FUNCLOG }

// 初期化
bool VideoSprite::init(const string& filename)
{
    av_register_all();
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        CCLOG("Could not initialize SDL - %s\n", SDL_GetError());
        return false;
    }
    
    // ファイルのフルパスを取得
    string fullPathForFilename { FileUtils::getInstance()->fullPathForFilename(filename) };
    
    // ファイルを開く
    AVFormatContext* fctx;
    if(avformat_open_input(&fctx, fullPathForFilename.c_str(), nullptr, nullptr));
    if(av_find_default_stream_index(fctx) < 0);
    
    // Retrieve stream information
    if(avformat_find_stream_info(fctx, nullptr) < 0) return false; // Couldn't find stream information
    
    // Dump information about file onto standard error
    // av_dump_format(fctx, 0, argv[1], 0);
    
    // 映像、音声ストリームを探す
    int videoStream = -1;
    int audioStream = -1;
    for (int i = 0; i < fctx->nb_streams; i++) {
        if (fctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO && videoStream < 0) {
            videoStream = i;
        }
        
        if (fctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO && audioStream < 0) {
            audioStream = i;
        }
    }
    
    if (videoStream == -1) return false;
    if (audioStream == -1) return false;
    
    AVCodecContext* aCodecCtxOrig { fctx->streams[audioStream]->codec };
    
    AVCodec* aCodec { avcodec_find_decoder(aCodecCtxOrig->codec_id) };
    if (!aCodec) {
        CCLOG("Unsupported codec!\n");
        return false;
    }

    // Copy context
    AVCodecContext* aCodecCtx { avcodec_alloc_context3(aCodec) };
    if (avcodec_copy_context(aCodecCtx, aCodecCtxOrig) != 0) {
        CCLOG("Couldn't copy codec context");
        return false; // Error copying codec context
    }
    
    // Set audio settings from codec info
    SDL_Texture* bmp;
    SDL_Surface* screen;
    SDL_Rect rect;
    SDL_Event event;
    SDL_AudioSpec wantedSpec, spec;
    
    wantedSpec.freq = aCodecCtx->sample_rate;
    wantedSpec.format = AUDIO_S16SYS;
    wantedSpec.channels = aCodecCtx->channels;
    wantedSpec.silence = 0;
    wantedSpec.samples = SDL_AUDIO_BUFFER_SIZE;
    wantedSpec.callback = nullptr;
    wantedSpec.userdata = aCodecCtx;
    
    if(SDL_OpenAudio(&wantedSpec, &spec) < 0) {
        CCLOG("SDL_OpenAudio: %s\n", SDL_GetError());
        return false;
    }
    
    avcodec_open2(aCodecCtx, aCodec, nullptr);
    
    return true;
}

void VideoSprite::audioCallback(void *userdata, Uint8* stream, int len)
{
    AVCodecContext* aCodecCtx = (AVCodecContext*)userdata;
    int len1, audio_size;
    
    static uint8_t audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    static unsigned int audio_buf_size = 0;
    static unsigned int audio_buf_index = 0;
    
    while (len > 0) {
        if (audio_buf_index >= audio_buf_size) {
            /* We have already sent all our data; get more */
            audio_size = audio_decode_frame(aCodecCtx, audio_buf, sizeof(audio_buf));
            if (audio_size < 0) {
                /* If error, output silence */
                audio_buf_size = 1024; // arbitrary?
                memset(audio_buf, 0, audio_buf_size);
            } else {
                audio_buf_size = audio_size;
            }
            audio_buf_index = 0;
        }
        len1 = audio_buf_size - audio_buf_index;
        if (len1 > len) len1 = len;
        memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
        len -= len1;
        stream += len1;
        audio_buf_index += len1;
    }
}
