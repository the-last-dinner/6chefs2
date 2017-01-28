//
//  VideoSprite.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/06/05.
//
//

#include "UI/Video/VideoSprite.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

// コンストラクタ
VideoSprite::VideoSprite() { FUNCLOG }

// デストラクタ
VideoSprite::~VideoSprite() { FUNCLOG }

// 初期化
bool VideoSprite::init(const string& filename)
{
    av_register_all();
    
    // ファイルのフルパスを取得
    string fullPathForFilename { FileUtils::getInstance()->fullPathForFilename(filename) };
    
    // ファイルを開く
    AVFormatContext* fctx;
    if(avformat_open_input(&fctx, fullPathForFilename.c_str(), nullptr, nullptr));
    if(av_find_default_stream_index(fctx) < 0);
    
    // 音声ストリームを探す
    int nb_streams = fctx->nb_streams;
    int audio_stream_index = -1;
    for(int i = 0; i < nb_streams; ++i)
    {
        if (fctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            audio_stream_index = i;
            break;
        }
    }
    if(audio_stream_index < 0);
    
    return true;
}
