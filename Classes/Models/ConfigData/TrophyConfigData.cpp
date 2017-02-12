//
//  TrophyConfigData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/12.
//
//

#include "Models/ConfigData/TrophyConfigData.h"

#include "Utils/JsonUtils.h"
#include "Utils/AssertUtils.h"

// 定数
const char* TrophyConfigData::FAST_CLEAR {"fast_clear"};
const char* TrophyConfigData::NO_SAVE_CLEAR {"no_save_clear"};
const char* TrophyConfigData::CHIKEN_SAVE {"chiken_save"};
const char* TrophyConfigData::TROPHY_COMPLETE {"trophy_complete"};
const char* TrophyConfigData::TROPHY_ID {"trophyID"};
const char* TrophyConfigData::COUNT {"count"};
const char* TrophyConfigData::TIME {"time"};

// コンストラクタ
bool TrophyConfigData::init()
{
    FUNCLOG
    this->filePath = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::TROPHY_CONFIG);
    if (this->filePath == "")
    {
        LastSupper::AssertUtils::fatalAssert(Resource::ConfigFiles::TROPHY_CONFIG + "is missing.");
        return false;
    }
    this->trophyConfig = LastSupper::JsonUtils::readJsonCrypted(this->filePath);
    return true;
}

// 設定されているtrophy_idの取得
int TrophyConfigData::getTrophyId(const char *targetTrophy)
{
    return this->getTrophyData(targetTrophy)[TROPHY_ID].GetInt();
}

// 設定されているトロフィーのプロパティを取得
int TrophyConfigData::getTrophyIntegerProperty(const char *targetTrophy, const char *targetProperty)
{
    return this->getTrophyData(targetTrophy)[targetProperty].GetInt();
}

// トロフィー設定データの取得
rapidjson::Value& TrophyConfigData::getTrophyData(const char *targetTrophy)
{
    if (!this->trophyConfig.HasMember(targetTrophy)) {
        LastSupper::AssertUtils::fatalAssert("targetTrophy is missng.");
        return this->trophyConfig;
    }
    return this->trophyConfig[targetTrophy];
}
