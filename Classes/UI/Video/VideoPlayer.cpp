//
//  VideoPlayer.cpp
//  6chefs2
//
//  Created by Sota Inami on 2017/2/12.
//
//

#include "UI/Video/VideoPlayer.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// 定数
const string VideoPlayer::DISP_VIDEO_PATH = "video/";
const float VideoPlayer::SHOW_DURATION = 0.1f;
const float VideoPlayer::HIDE_DURATION = 0.1f;

// コンストラクタ
VideoPlayer::VideoPlayer() {FUNCLOG};

//デストラクタ
VideoPlayer::~VideoPlayer() {FUNCLOG};

// create関数
VideoPlayer* VideoPlayer::create(const string& fileName, const bool skip, function<void()> onEnd)
{
    VideoPlayer* p {new(nothrow) VideoPlayer()};
    
    if(p && p->init(fileName, skip, onEnd)) {
        CC_SAFE_RETAIN(p);
        return p;
    } else {
        delete p;
        p = nullptr;
        return nullptr;
    }
}

// 初期化
bool VideoPlayer::init(const string& fileName, const bool skip, function<void()> onEnd)
{
    if(!Layer::init()) return false;
    
    // charに以下のように変換しないと動画がデコードされない
    string filePath = DISP_VIDEO_PATH + fileName;
    int len = filePath.length();
    char* fname = new char[len+1];
    memcpy(fname, filePath.c_str(), len+1);
    
    VideoSprite *video = VideoSprite::create(fname);
    video->setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    // 再生が終わった時
    video->setVideoEndCallback([this, video, onEnd](){
        video->stopVideo();
        this->remove(onEnd);
    });
    this->addChild(video);
    video->playVideo();
    
    // フェードイン
    this->runAction(Sequence::createWithTwoActions(FadeIn::create(SHOW_DURATION), CallFunc::create([this, onEnd, skip, video]{
        // フェードイン後
        // スキップ可能ならキーボード操作で自身を消す
        if(skip) {
            // リスナ生成
            EventListenerKeyboardLayer* listener { EventListenerKeyboardLayer::create() };
            listener->onEnterKeyPressed = [this, video, onEnd]{
                video->stopVideo();
                this->remove(onEnd);
            };
            this->addChild(listener);
            
            return;
        }
    })));
    
    return true;
}

// 消す
void VideoPlayer::remove(function<void()> callback)
{
    this->runAction(Sequence::create(CallFunc::create(callback), RemoveSelf::create(), nullptr));
}
