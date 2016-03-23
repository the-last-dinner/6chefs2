//
//  GameOverScene.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#include "Scenes/GameOverScene.h"

#include "Datas/Scene/GameOverSceneData.h"

#include "Layers/LoadingLayer.h"
#include "Layers/EventListener/ConfigEventListenerLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"

#include "Scenes/TitleScene.h"

// 定数
const float GameOverScene::CHAR_ANIMATION_DURATION = 4.f;
const float GameOverScene::CHAR_ANIMATION_LATENCY = 0.1f;

// コンストラクタ
GameOverScene::GameOverScene() {FUNCLOG};

// デストラクタ
GameOverScene::~GameOverScene() {FUNCLOG};

// 初期化
bool GameOverScene::init(const Type type)
{
    SoundManager::getInstance()->playSE("blood.mp3");
    
    Sprite* blood {Sprite::create()};
    blood->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    blood->setColor(Color3B(177, 0, 0));
    blood->setPosition(WINDOW_CENTER);
    this->addChild(blood, Priority::TOP_COVER);
    this->bloodCover = blood;
    
    if(!BaseScene::init(GameOverSceneData::create())) return false;
    
    this->configListener->setKeyconfigEnabled(false);
    
    return true;
}

// シーン切り替え完了時
void GameOverScene::onEnter()
{
    BaseScene::onEnter();
}

// リソースのプリロード完了時
void GameOverScene::onPreloadFinished(LoadingLayer* loadingLayer)
{
    loadingLayer->onLoadFinished();
    
    this->bloodCover->runAction(Sequence::createWithTwoActions(FadeOut::create(0.4f), RemoveSelf::create()));
    
    SoundManager::getInstance()->playBGM("gameover.mp3", false);
    
    Sprite* bloodFrame {Sprite::createWithSpriteFrameName("blood2.png")};
    bloodFrame->setPosition(WINDOW_CENTER);
    bloodFrame->setScale(bloodFrame->getContentSize().width / WINDOW_WIDTH, bloodFrame->getContentSize().height / WINDOW_HEIGHT);
    this->addChild(bloodFrame);
    
    Label* gameover { Label::createWithTTF("Game Over", Resource::Font::SYSTEM, 120.f) };
    gameover->setPosition(WINDOW_CENTER);
    this->addChild(gameover);
    
    vector<Vec2> idxToVec
    {
        Vec2(-120, 40),
        Vec2(-120, -50),
        Vec2(-70, 0),
        Vec2(-50, 100),
        Vec2::ZERO,
        Vec2(0, -50),
        Vec2(70, 50),
        Vec2(100, 100),
        Vec2(70, 0),
    };
    
    // 文字のアニメーション
    int charNum {gameover->getStringLength()};
    for(int i { 0 }; i < charNum; i++)
    {
        Sprite* c { gameover->getLetter(i) };
        c->setOpacity(0);
        c->runAction(Sequence::createWithTwoActions(DelayTime::create(CHAR_ANIMATION_LATENCY * i), Spawn::createWithTwoActions(FadeIn::create(CHAR_ANIMATION_DURATION), MoveBy::create(CHAR_ANIMATION_DURATION, idxToVec[i]))));
    }
    
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(CHAR_ANIMATION_DURATION + CHAR_ANIMATION_LATENCY * charNum), CallFunc::create(CC_CALLBACK_0(GameOverScene::onAnimationFinished, this))));
}

// アニメーションが終了した時
void GameOverScene::onAnimationFinished()
{
    // キーボードリスナ生成
    EventListenerKeyboardLayer* listener {EventListenerKeyboardLayer::create()};
    this->addChild(listener);
    
    listener->onEnterKeyPressed = []{SoundManager::getInstance()->stopBGMAll(); Director::getInstance()->replaceScene(TitleScene::create());};
}
