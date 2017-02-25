//
//  OpeningScene.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/13.
//
//

#include "Scenes/OpeningScene.h"

#include "Scenes/TitleScene.h"
#include "Datas/Scene/OpeningSceneData.h"
#include "Layers/EventListener/ConfigEventListenerLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "Layers/LoadingLayer.h"
#include "UI/Video/VideoPlayer.h"

// 初期化
bool OpeningScene::init()
{
    if (!BaseScene::init(OpeningSceneData::create())) return false;
    
    MasterConfigData* masterConfigData { ConfigDataManager::getInstance()->getMasterConfigData() };
    _videoFileName = masterConfigData->getString(MasterConfigData::OPENING_VIDEO_FILE);
    _bgmFileName = masterConfigData->getString(MasterConfigData::OPENING_BGM_FILE);
    
    return true;
}

// シーン生成直後
void OpeningScene::onEnter()
{
    BaseScene::onEnter();
}

// データ読み込み後
void OpeningScene::onPreloadFinished(LoadingLayer *loadingLayer)
{
    // ローディング終了
    loadingLayer->onLoadFinished();
    
    this->setVideoLayer();
    //this->setVideoLayerDummy();
    this->setVideoBgm();
    //this->setPressEnter();
}

// 動画レイヤーをセット
void OpeningScene::setVideoLayer()
{
     VideoPlayer* videoPlayer { VideoPlayer::create(_videoFileName, true, [this]{
            SoundManager::getInstance()->stopBGMAll();
            Director::getInstance()->replaceScene(TitleScene::create());
         }) };
     this->addChild(videoPlayer);
}

// 動画レイヤーのダミー(テスト用に残しておく)
void OpeningScene::setVideoLayerDummy()
{
    // ダミー動画レイヤー
    Label* videoLayer {Label::createWithTTF("動画再生なう", Resource::Font::MESSAGE, 40)};
    videoLayer->setPosition(WINDOW_CENTER);
    this->addChild(videoLayer);
    
    // キーボードリスナ生成(Videoのコールバックにするからあとで消す)
    EventListenerKeyboardLayer* listener {EventListenerKeyboardLayer::create()};
    listener->onEnterKeyPressed = []{
        SoundManager::getInstance()->stopBGMAll();
        Director::getInstance()->replaceScene(TitleScene::create());
    };
    this->addChild(listener);
}

// BGMのセット
void OpeningScene::setVideoBgm()
{
    SoundManager::getInstance()->playBGM(_bgmFileName, false, 2.5);
}

// PRESS ENTERをセット
void OpeningScene::setPressEnter()
{
    // PRESS ENTER
    Label* pressEnter {Label::createWithTTF("PRESS ENTER", Resource::Font::MESSAGE, 18)};
    pressEnter->setPosition(Point(WINDOW_WIDTH/2, pressEnter->getContentSize().height * 2));
    pressEnter->setOpacity(0);
    pressEnter->setLocalZOrder(999);
    this->addChild(pressEnter);
    
    // 点滅を設定
    ActionInterval* flashing = Sequence::createWithTwoActions(
        TargetedAction::create(pressEnter, FadeTo::create(0.6f, 200)),
        TargetedAction::create(pressEnter, FadeTo::create(0.6f, 30))
    );
    this->runAction(RepeatForever::create(flashing));
}
