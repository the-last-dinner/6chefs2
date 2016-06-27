//
//  DebugManager.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/06.
//
//

#include "Managers/DebugManager.h"
#include "Utils/JsonUtils.h"

// 定数
const char* DebugManager::STATS {"stats"};
const char* DebugManager::DEBUG_MASK {"debug_mask"};
const char* DebugManager::INVINCIBLE_MODE {"invincible_mode"};
const char* DebugManager::PLAIN_DATA {"plain_data"};
const char* DebugManager::CRYPT_TRIGGER {"crypt_trigger"};

// 唯一のインスタンスを初期化
static DebugManager* _instance = nullptr;

#pragma mark Core

// インスタンスの生成&取得
DebugManager* DebugManager::getInstance()
{
    if(!_instance) _instance = new DebugManager();
    return _instance;
}

// インスタンスの消去
void DebugManager::destroy()
{
    delete _instance;
}

// デストラクタ
DebugManager::~DebugManager(){FUNCLOG}

// コンストラクタ
DebugManager::DebugManager()
{
    // return;
    FUNCLOG
    this->file_path = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::DEBUG_CONFIG);
    if (this->file_path == "") return;
    this->debugConfig = LastSupper::JsonUtils::readJsonFile(this->file_path);
    this->hasDebugConfig = true;
}

// DebugConfigの書き出し
void DebugManager::writeConfig()
{
    LastSupper::JsonUtils::writeJsonFile(this->file_path, this->debugConfig);
}

// スタッツを表示するかどうか
bool DebugManager::displayStats()
{
    if (!this->hasDebugConfig) return false;
    if (!this->debugConfig.HasMember(STATS)) return false;
    return this->debugConfig[STATS].GetBool();
}

// MapのDebugMaskを表示するかどうか
bool DebugManager::displayDebugMask()
{
    if (!this->hasDebugConfig) return false;
    if (!this->debugConfig.HasMember(DEBUG_MASK)) return false;
    return this->debugConfig[DEBUG_MASK].GetBool();
}

// 無敵モードかどうか
bool DebugManager::isInvincibleMode()
{
    if (!this->hasDebugConfig) return false;
    if (!this->debugConfig.HasMember(INVINCIBLE_MODE)) return false;
    return this->debugConfig[INVINCIBLE_MODE].GetBool();
}

// データが平文かどうか
bool DebugManager::isPlainData()
{
    if (!this->hasDebugConfig) return false;
    if (!this->debugConfig.HasMember(PLAIN_DATA)) return false;
    return this->debugConfig[PLAIN_DATA].GetBool();
}

// データを平文じゃないに設定
void DebugManager::setOffPlainData()
{
    if (!this->hasDebugConfig) return;
    if (!this->debugConfig.HasMember(PLAIN_DATA)) return;
    this->debugConfig[PLAIN_DATA].SetBool(false);
    this->writeConfig();
}

// 暗号化トリガーが立っているかどうか
bool DebugManager::getCryptTrigger()
{
    if (!this->hasDebugConfig) return false;
    if (!this->debugConfig.HasMember(CRYPT_TRIGGER)) return false;
    return this->debugConfig[CRYPT_TRIGGER].GetBool();
}

// 暗号化トリガーをオフに設定
void DebugManager::setOffCryptTrigger()
{
    if (!this->hasDebugConfig) return;
    if (!this->debugConfig.HasMember(CRYPT_TRIGGER)) return;
    this->debugConfig[CRYPT_TRIGGER].SetBool(false);
    this->writeConfig();
}