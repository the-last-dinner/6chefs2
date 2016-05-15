//
//  RootScene.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/14.
//
//

#include "Scenes/RootScene.h"

#include "Layers/EventListener/ConfigEventListenerLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"

#include "Managers/SceneManager.h"

#include "Scenes/StartUpScene.h"

# pragma mark private

// コンストラクタ
RootScene::RootScene() { FUNCLOG };

// デストラクタ
RootScene::~RootScene() { FUNCLOG };

// 初期化
bool RootScene::init()
{
    if(!Scene::init()) return false;
    
    EventListenerKeyboardLayer* listener { EventListenerKeyboardLayer::create() };
    this->addChild(listener);
    this->listener = listener;
    
    // コンフィグ用のイベントリスナーを登録
    ConfigEventListenerLayer* configListener { ConfigEventListenerLayer::create() };
    this->addChild(configListener);
    this->configListener = configListener;
    
    return true;
}

void RootScene::onEnter()
{
    Scene::onEnter();
    
    SceneManager::getInstance()->runWithScene(StartUpScene::create());
}

# pragma mark -
# pragma mark getter

ConfigEventListenerLayer* RootScene::getConfigEventListener() const
{
    return this->configListener;
}

EventListenerKeyboardLayer* RootScene::getEventListenerKeyboard() const
{
    return this->listener;
}
