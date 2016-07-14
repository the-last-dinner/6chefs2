//
//  AssertScene.cpp
//  6chefs2
//
//  Created by S.inami on 2016/07/13.
//
//

#include "AssertScene.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
AssertScene::AssertScene(){FUNCLOG}

// デストラクタ
AssertScene::~AssertScene(){FUNCLOG}

// 初期化
bool AssertScene::init(const string& message, const bool& hideable)
{
    FUNCLOG;
    // カウントダウンしてれば停止
    DungeonSceneManager::getInstance()->pauseStopWatch();
    
    // 黒い画面を生成
    auto layer = LayerColor::create(Color4B::BLACK);
    // 画面サイズ取得
    Size winSize = Director::getInstance()->getWinSize();
    // メッセージを追加
    Label* text { Label::createWithSystemFont(message, "fonts/mgenplus-1c-light.ttf", 18.f, winSize) };
    text->setColor(Color3B::WHITE);
    text->setPosition(Point(winSize.width / 2, winSize.height / 2));
    layer->addChild(text);
    // 左上に寄せる
    text->setHorizontalAlignment(TextHAlignment::LEFT);
    text->setVerticalAlignment(TextVAlignment::TOP);
    
    if(hideable)
    {
        // 戻る用のメッセージ表示
        Label* hideText {Label::createWithTTF("X:戻る", "fonts/mgenplus-1c-light.ttf", 18.f, winSize)};
        hideText->setColor(Color3B::WHITE);
        hideText->setPosition(Point(winSize.width / 2, winSize.height / 2));
        layer->addChild(hideText);
        // 中央下に寄せる
        hideText->setHorizontalAlignment(TextHAlignment::CENTER);
        hideText->setVerticalAlignment(TextVAlignment::BOTTOM);
        // キーボードのリスナーを貼る
        EventListenerKeyboardLayer* listenerKeyboard { EventListenerKeyboardLayer::create() };
        this->addChild(listenerKeyboard);
        listenerKeyboard->onMenuKeyPressed = CC_CALLBACK_0(AssertScene::onMenuKeyPressed, this);
        listenerKeyboard->setInputCheckDelay(0.3f);
        listenerKeyboard->setInputCheckInterval(0.1f);
        listenerKeyboard->setEnabled(true);
        this->listenerKeyboard = listenerKeyboard;

    }
    this->addChild(layer);
    
    return true;
}


void AssertScene::onPreloadFinished(LoadingLayer* loadingLayer){}

// 方向キーを押した時
void AssertScene::onCursorKeyPressed(const Key& key){}

// 決定キーを押した時
void AssertScene::onEnterKeyPressed(){}

// キーを押し続けている時
void AssertScene::intervalInputCheck(const vector<Key>& keys){}

// メニューキー押したとき
void AssertScene::onMenuKeyPressed()
{
    // カウントダウンをしれてば再開
    DungeonSceneManager::getInstance()->startStopWatch();
    Director::getInstance()->popScene();
}




