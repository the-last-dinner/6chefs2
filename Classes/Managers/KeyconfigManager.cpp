//
//  KeyconfigManager.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/03/12.
//
//

#include "Managers/KeyconfigManager.h"

#include "Layers/Menu/KeyconfigMenuLayer.h"

// 定数
const map<KeyconfigManager::DashKeyType, string> KeyconfigManager::DashKeyTypeToStr
{
    {DashKeyType::LEFT_SHIFT, "左shift"},
    {DashKeyType::RIGHT_SHIFT, "右shift"},
};

const map<KeyconfigManager::CursorKeyType, string> KeyconfigManager::CursorKeyTypeToStr
{
    {CursorKeyType::ARROW, "カーソルキー"},
    {CursorKeyType::WASD, "WASD"},
    {CursorKeyType::VIM, "VimMode"},
};

const map<KeyconfigManager::DashKeyType, EventKeyboard::KeyCode> KeyconfigManager::dashKeys
{
    {DashKeyType::LEFT_SHIFT, EventKeyboard::KeyCode::KEY_LEFT_SHIFT},
    {DashKeyType::RIGHT_SHIFT, EventKeyboard::KeyCode::KEY_RIGHT_SHIFT},
};

const map<KeyconfigManager::CursorKeyType, KeyconfigManager::Keyconfig> KeyconfigManager::cursorKeys
{
    {CursorKeyType::ARROW,
        {
            {EventKeyboard::KeyCode::KEY_UP_ARROW, Key::UP},
            {EventKeyboard::KeyCode::KEY_LEFT_ARROW, Key::LEFT},
            {EventKeyboard::KeyCode::KEY_DOWN_ARROW, Key::DOWN},
            {EventKeyboard::KeyCode::KEY_RIGHT_ARROW, Key::RIGHT},
        }
    },
    {CursorKeyType::WASD,
        {
            {EventKeyboard::KeyCode::KEY_W, Key::UP},
            {EventKeyboard::KeyCode::KEY_A, Key::LEFT},
            {EventKeyboard::KeyCode::KEY_S, Key::DOWN},
            {EventKeyboard::KeyCode::KEY_D, Key::RIGHT},
        }
    },
    {CursorKeyType::VIM,
        {
            {EventKeyboard::KeyCode::KEY_K, Key::UP},
            {EventKeyboard::KeyCode::KEY_H, Key::LEFT},
            {EventKeyboard::KeyCode::KEY_J, Key::DOWN},
            {EventKeyboard::KeyCode::KEY_L, Key::RIGHT},
        }
    }
};

// 唯一のインスタンス
KeyconfigManager* _instance { nullptr };

// インスタンスを取得
KeyconfigManager* KeyconfigManager::getInstance()
{
    if(!_instance) _instance = new KeyconfigManager();
    
    return _instance;
}

// インスタンスを破棄
void KeyconfigManager::destroy()
{
    delete _instance;
}

// コンストラクタ
KeyconfigManager::KeyconfigManager()
{
    FUNCLOG
    
    // 決定キー
    this->keyconfig[EventKeyboard::KeyCode::KEY_SPACE] = Key::ENTER;
    this->keyconfig[EventKeyboard::KeyCode::KEY_ENTER] = Key::ENTER;
    this->keyconfig[EventKeyboard::KeyCode::KEY_Z]     = Key::ENTER;
    
    // メニューキー
    this->keyconfig[EventKeyboard::KeyCode::KEY_X] = Key::MENU;
    
    // キーコンフィグキー
    this->keyconfig[EventKeyboard::KeyCode::KEY_F1] = Key::KEY_CONF;
    
    // 画面設定キー
    this->keyconfig[EventKeyboard::KeyCode::KEY_F4] = Key::WIN_SIZE;
}

// デストラクタ
KeyconfigManager::~KeyconfigManager() { FUNCLOG };

// ダッシュキーを設定
void KeyconfigManager::setDashKey(const DashKeyType keyType)
{
    for(pair<DashKeyType, EventKeyboard::KeyCode> typeToKey : dashKeys)
    {
        this->keyconfig.erase(typeToKey.second);
    }
    
    this->dashKeyType = keyType;
    this->keyconfig[dashKeys.at(keyType)] = Key::DASH;
}

// 方向キーを設定
void KeyconfigManager::setCursorKey(const CursorKeyType keyType)
{
    for(pair<CursorKeyType, Keyconfig> typeToConf : cursorKeys)
    {
        for(auto conf : typeToConf.second)
        {
            this->keyconfig.erase(conf.first);
        }
    }
    
    this->cursorKeyType = keyType;
    Keyconfig config { cursorKeys.at(keyType) };
    
    for(auto conf : config)
    {
        this->keyconfig.insert(conf);
    }
}

// cocos2d上でのキーをゲーム上のキーに変換
Key KeyconfigManager::convertKeyCode(const EventKeyboard::KeyCode keyCode) const
{
    return (keyconfig.count(keyCode) == 0)? Key::SIZE : keyconfig.at(keyCode);
}

// キーコンフィグが開かれているかどうか
bool KeyconfigManager::isKeyconfigOpened() const
{
    return this->menuLayer;
}

// キーコンフィグを開く
void KeyconfigManager::openKeyconfigMenu(function<void()> onClose)
{
    if(this->menuLayer) return;
    
    KeyconfigMenuLayer* menuLayer { KeyconfigMenuLayer::create() };
    menuLayer->onClose = [this, onClose](KeyconfigMenuLayer* layer)
    {
        if(onClose) onClose();
        layer->removeFromParent();
        this->menuLayer = nullptr;
    };
    Director::getInstance()->getRunningScene()->addChild(menuLayer);
    this->menuLayer = menuLayer;
}

// 表示用
string KeyconfigManager::typeToDispName(const DashKeyType keyType)
{
    return DashKeyTypeToStr.at(keyType);
}

string KeyconfigManager::typeToDispName(const CursorKeyType keyType)
{
    return CursorKeyTypeToStr.at(keyType);
}
