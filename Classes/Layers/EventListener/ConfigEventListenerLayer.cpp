//
//  ConfigEventListenerLayer.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/03/23.
//
//

#include "Layers/EventListener/ConfigEventListenerLayer.h"

#include "Managers/KeyconfigManager.h"
#include "Managers/EventListenerKeyboardManager.h"

// コンストラクタ
ConfigEventListenerLayer::ConfigEventListenerLayer() { FUNCLOG };

// デストラクタ
ConfigEventListenerLayer::~ConfigEventListenerLayer() { FUNCLOG };

// 初期化
bool ConfigEventListenerLayer::init()
{
    if(!Layer::init()) return false;
    
    EventListenerKeyboard* listener { EventListenerKeyboard::create() };
    listener->onKeyPressed = CC_CALLBACK_1(ConfigEventListenerLayer::onKeyPressed, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

// キーを押した時
void ConfigEventListenerLayer::onKeyPressed(const EventKeyboard::KeyCode &keyCode)
{
    Key key { KeyconfigManager::getInstance()->convertKeyCode(keyCode) };
    
    if(key == Key::SIZE) return;
    
    map<Key, function<void()>> pFuncs
    {
        {Key::KEY_CONF, CC_CALLBACK_0(ConfigEventListenerLayer::openKeyconfigMenu, this)},
        {Key::WIN_SIZE, CC_CALLBACK_0(ConfigEventListenerLayer::switchWinSize, this)},
    };
    
    if(pFuncs.count(key) == 0) return;
    
    pFuncs.at(key)();
}

// キーコンフィグを開く
void ConfigEventListenerLayer::openKeyconfigMenu()
{
    if(!this->keyconfigEnabled) return;
    if(KeyconfigManager::getInstance()->isKeyconfigOpened()) return;
    if(this->onOpenkeyconfigMenu) this->onOpenkeyconfigMenu();
    EventListenerKeyboardManager::getInstance()->pauseAllEventListener(true);
    KeyconfigManager::getInstance()->openKeyconfigMenu([this]
    {
        EventListenerKeyboardManager::getInstance()->pauseAllEventListener(false);
        if(this->onKeyconfigMenuClosed) this->onKeyconfigMenuClosed();
    });
}

// 画面サイズを切り替え
void ConfigEventListenerLayer::switchWinSize()
{
    vector<float> config
    {
        0.75f,
        1.f,
        1.25f,
        1.5f,
    };
    
    int currentConfIdx { 0 };
    GLView* glView { Director::getInstance()->getOpenGLView() };
    float current { glView->getFrameSize().width / WINDOW_WIDTH };
    
    for(int i {0}; i < config.size(); i++)
    {
        if(config.at(i) != current) continue;
        currentConfIdx = i;
        break;
    }
    float ratio { config.at(currentConfIdx == config.size() - 1 ? 0 : currentConfIdx + 1) };
    
    glView->setFrameSize(WINDOW_WIDTH * ratio, WINDOW_HEIGHT * ratio);
    glView->setDesignResolutionSize(WINDOW_WIDTH, WINDOW_HEIGHT, ResolutionPolicy::NO_BORDER);
    Director::getInstance()->setOpenGLView(glView);
}

// キーコンフィグを有効・無効化
void ConfigEventListenerLayer::setKeyconfigEnabled(bool enabled) { this->keyconfigEnabled = enabled; }
