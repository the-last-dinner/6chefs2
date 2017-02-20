//
//  MasterConfigData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/28.
//
//

#include "Models/ConfigData/MasterConfigData.h"

#include "Utils/JsonUtils.h"
#include "Utils/AssertUtils.h"

// 定数
const char* MasterConfigData::VERSION {"version"};
const char* MasterConfigData::COPYRIGHT {"copyright"};
const char* MasterConfigData::DISPLAY {"display"};
const char* MasterConfigData::TWO_ICON {"two_icon"};
const char* MasterConfigData::FRIENDSHIP {"friendship"};
const char* MasterConfigData::SPECIAL_ROOM {"special_room"};
const char* MasterConfigData::OPENING_SCENE {"opening_scene"};
const char* MasterConfigData::OPENING_VIDEO_FILE {"opening_video_file"};
const char* MasterConfigData::OPENING_BGM_FILE {"opening_bgm_file"};

// コンストラクタ
bool MasterConfigData::init()
{
    FUNCLOG
    this->filePath = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::MASTER_CONFIG);
    if (this->filePath == "")
    {
        LastSupper::AssertUtils::fatalAssert(Resource::ConfigFiles::MASTER_CONFIG + "is missing.");
        return false;
    }
    this->masterConfig = LastSupper::JsonUtils::readJsonCrypted(this->filePath);
    return true;
}


// 文字列を取得
string MasterConfigData::getString(const char *targetProperty)
{
    if (!this->masterConfig.HasMember(VERSION)) {
        string msg = "MasterConfig property is missing.";
        LastSupper::AssertUtils::fatalAssert(msg);
        return msg;
    }
    return this->masterConfig[targetProperty].GetString();
}

// ディスプレイ系のコンフィグを取得
bool MasterConfigData::isDisplay(const char* targetProperty)
{
    if (!this->masterConfig.HasMember(DISPLAY)) return false;
    if (!this->masterConfig[DISPLAY].HasMember(targetProperty)) return false;
    return this->masterConfig[DISPLAY][targetProperty].GetBool();
}
