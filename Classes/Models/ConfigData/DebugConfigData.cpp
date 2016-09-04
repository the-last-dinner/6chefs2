//
//  DebugConfigData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/28.
//
//

#include "Models/ConfigData/DebugConfigData.h"

#include "Utils/AssertUtils.h"
#include "Utils/JsonUtils.h"

// 定数
const char* DebugConfigData::STATS {"stats"};
const char* DebugConfigData::DEBUG_MASK {"debug_mask"};
const char* DebugConfigData::INVINCIBLE_MODE {"invincible_mode"};
const char* DebugConfigData::PLAIN_DATA {"plain_data"};
const char* DebugConfigData::CRYPT_TRIGGER {"crypt_trigger"};

bool DebugConfigData::init()
{
    FUNCLOG
    this->filePath = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::DEBUG_CONFIG);
    if (this->filePath == "") return true;
    this->debugConfig = LastSupper::JsonUtils::readJsonFile(this->filePath);
    this->hasDebugConfig = true;
    return true;
}

// DebugConfigデータの取得(boolean型)
bool DebugConfigData::getBoolValue(const char* key)
{
    if (!this->hasDebugConfig) return false;
    
    if (!this->debugConfig.HasMember(key))
    {
        LastSupper::AssertUtils::warningAssert("DebugConfigError\nkey: " + to_string(*key) + "is missing.");
        return false;
    }
    
    if (!this->debugConfig[key].IsBool())
    {
        LastSupper::AssertUtils::fatalAssert("DebugConfigError\nkey: " + to_string(*key) + "must be bool!");
        return false;
    }
    
    return this->debugConfig[key].GetBool();
}

// デバッグモードかどうか
bool DebugConfigData::isDebugMode()
{
    return this->getBoolValue(DebugConfigData::PLAIN_DATA);
}

// 暗号初期化が必要かどうか
bool DebugConfigData::needInitialCrypt()
{
    return (this->getBoolValue(DebugConfigData::PLAIN_DATA) && this->getBoolValue(DebugConfigData::CRYPT_TRIGGER));
}

// 暗号化が必要かどうか
bool DebugConfigData::needCrypt()
{
    return (!this->getBoolValue(DebugConfigData::PLAIN_DATA) || this->getBoolValue(DebugConfigData::CRYPT_TRIGGER));
}

// 暗号化したことを記録
void DebugConfigData::setCrypted()
{
    if (!this->hasDebugConfig) return;
    
    if (this->debugConfig.HasMember(PLAIN_DATA))
    {
        this->debugConfig[PLAIN_DATA].SetBool(false);
    }
    
    if (this->debugConfig.HasMember(CRYPT_TRIGGER))
    {
        this->debugConfig[CRYPT_TRIGGER].SetBool(false);
    }
    
    this->writeConfig();
}

// DebugConfigの書き出し
void DebugConfigData::writeConfig()
{
    LastSupper::JsonUtils::writeJsonFile(this->filePath, this->debugConfig);
}
