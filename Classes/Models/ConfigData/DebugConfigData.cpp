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

bool DebugConfigData::init()
{
    FUNCLOG
    this->filePath = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::DEBUG_CONFIG);
    if (this->filePath == "") return true;
    this->debugConfig = LastSupper::JsonUtils::readJsonCrypted(this->filePath);
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
    return !IS_ENCRYPTED;
}

// DebugConfigの書き出し
void DebugConfigData::writeConfig()
{
    LastSupper::JsonUtils::writeJsonFile(this->filePath, this->debugConfig);
}
