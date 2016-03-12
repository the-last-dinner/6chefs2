//
//  KeyconfigManager.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/03/12.
//
//

#include "Managers/KeyconfigManager.h"

// 定数
const map<string, KeyconfigManager::EnterKeyType> KeyconfigManager::strToEnterKeyType
{
    {"space", EnterKeyType::SPACE},
    {"z", EnterKeyType::Z},
    {"enter", EnterKeyType::ENTER},
};

const map<string, KeyconfigManager::DashKeyType> KeyconfigManager::strToDashKeyType
{
    {"left", DashKeyType::LEFT_SHIFT},
    {"right", DashKeyType::RIGHT_SHIFT},
};

const map<string, KeyconfigManager::CursorKeyType> KeyconfigManager::strToCursorKeyType
{
    {"arrow", CursorKeyType::ARROW},
    {"wasd", CursorKeyType::WASD},
};

const map<KeyconfigManager::EnterKeyType, EventKeyboard::KeyCode> KeyconfigManager::enterKeys
{
    {EnterKeyType::SPACE, EventKeyboard::KeyCode::KEY_SPACE},
    {EnterKeyType::Z, EventKeyboard::KeyCode::KEY_Z},
    {EnterKeyType::ENTER, EventKeyboard::KeyCode::KEY_ENTER},
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
    this->setEnterKey(EnterKeyType::SPACE);
    
    // ダッシュキー
    this->setDashKey(DashKeyType::LEFT_SHIFT);
    
    // 方向キー
    this->setCursorKey(CursorKeyType::ARROW);
    
    // メニューキー
    this->keyconfig[EventKeyboard::KeyCode::KEY_X] = Key::MENU;
    
    // キーコンフィグキー
    this->keyconfig[EventKeyboard::KeyCode::KEY_F8] = Key::KEY_CONF;
    
    // 画面設定キー
    this->keyconfig[EventKeyboard::KeyCode::KEY_F4] = Key::WIN_SIZE;
}

// デストラクタ
KeyconfigManager::~KeyconfigManager() { FUNCLOG };

// 決定キーを設定
void KeyconfigManager::setEnterKey(const EnterKeyType keyType)
{
    for(pair<EnterKeyType, EventKeyboard::KeyCode> typeToKey : enterKeys)
    {
        this->keyconfig.erase(typeToKey.second);
    }
    
    this->keyconfig[enterKeys.at(keyType)] = Key::ENTER;
}

// ダッシュキーを設定
void KeyconfigManager::setDashKey(const DashKeyType keyType)
{
    for(pair<DashKeyType, EventKeyboard::KeyCode> typeToKey : dashKeys)
    {
        this->keyconfig.erase(typeToKey.second);
    }
    
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
