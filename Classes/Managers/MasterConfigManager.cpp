//
//  MasterConfigManager.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/18.
//
//

#include "Managers/MasterConfigManager.h"
#include "Utils/JsonUtils.h"
#include "Utils/AssertUtils.h"

// 定数
const char* MasterConfigManager::DISPLAY {"display"};
const char* MasterConfigManager::FRIENDSHIP {"friendship"};

// 唯一のインスタンスを初期化
static MasterConfigManager* _instance = nullptr;

#pragma mark Core

// インスタンスの生成&取得
MasterConfigManager* MasterConfigManager::getInstance()
{
    if(!_instance) _instance = new MasterConfigManager();
    return _instance;
}

// インスタンスの消去
void MasterConfigManager::destroy()
{
    delete _instance;
}

// デストラクタ
MasterConfigManager::~MasterConfigManager(){FUNCLOG}

// コンストラクタ
MasterConfigManager::MasterConfigManager()
{
    FUNCLOG
    this->file_path = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::MASTER_CONFIG);
    if (this->file_path == "")
    {
        LastSupper::AssertUtils::fatalAssert(Resource::ConfigFiles::MASTER_CONFIG + "is missing.");
        return;
    }
    this->masterConfig = LastSupper::JsonUtils::readJsonFile(this->file_path);
}

// ディスプレイ系のコンフィグを取得
bool MasterConfigManager::isDisplay(const char* targetProperty)
{
    if (!this->masterConfig.HasMember(DISPLAY)) return false;
    if (!this->masterConfig[DISPLAY].HasMember(targetProperty)) return false;
    return this->masterConfig[DISPLAY][targetProperty].GetBool();
}

