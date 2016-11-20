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
bool AssertScene::init(const string& message, const AssertType& assertType)
{
    FUNCLOG;
    // タイトルと背景色を決定
    string title = "";
    Color4B color {};
    bool hideable;
    switch (assertType) {
        case AssertType::INFO:
            title = "InfoAssert";
            color = Color4B::BLUE;
            hideable = true;
            break;
        case AssertType::WARNING:
            title = "WarningAssert";
            color = Color4B::ORANGE;
            hideable = true;
            break;
        case AssertType::FATAL:
            title = "FatalAssert";
            color = Color4B::BLACK;
            hideable = false;
            break;
        case AssertType::ERROR:
            title = "SystemError";
            color = Color4B::BLACK;
            hideable = false;
            break;
        default:
            break;
    }
    
    // レイヤーを生成
    auto layer = LayerColor::create(color);
    // 画面サイズ取得
    Size winSize = Director::getInstance()->getWinSize();
    
    // タイトルを追加
    Label* titleLabel {Label::createWithTTF(title, "fonts/mgenplus-1c-light.ttf", 36.f)};
    titleLabel->setPosition(WINDOW_WIDTH / 2, winSize.height - titleLabel->getContentSize().height * 2);
    layer->addChild(titleLabel);
    
    // メッセージを追加
    Label* messageLabel { Label::createWithTTF(message, "fonts/mgenplus-1c-light.ttf", 24.f) };
    messageLabel->setLineHeight(45.f);
    messageLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    messageLabel->setVerticalAlignment(TextVAlignment::CENTER);
    messageLabel->setPosition(messageLabel->getContentSize().width / 2 + WINDOW_WIDTH / 5, layer->getContentSize().height / 2);
    layer->addChild(messageLabel);
    
    if (hideable) {
        // 戻る用のメッセージ表示
        Label* hideLabel {Label::createWithTTF("X:戻る", "fonts/mgenplus-1c-light.ttf", 24.f, winSize)};
        hideLabel->setColor(Color3B::WHITE);
        hideLabel->setPosition(Point(winSize.width / 2, winSize.height / 2));
        layer->addChild(hideLabel);
        // 中央下に寄せる
        
        hideLabel->setHorizontalAlignment(TextHAlignment::CENTER);
        hideLabel->setVerticalAlignment(TextVAlignment::BOTTOM);
        
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

// pushされた時
void AssertScene::onEnter()
{
    Scene::onEnter();
    this->onEnterPushedScene();
}

// popされた時
void AssertScene::onExit()
{
    Scene::onExit();
    this->onExitPushedScene();
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
    this->listenerKeyboard->setEnabled(false);
    Director::getInstance()->popScene();
}




