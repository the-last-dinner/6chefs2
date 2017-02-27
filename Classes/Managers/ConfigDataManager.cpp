//
//  ConfigDataManager.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/18.
//
//

#include "Managers/ConfigDataManager.h"

#include "Utils/JsonUtils.h"
#include "Utils/AssertUtils.h"

#pragma mark Core

// 唯一のインスタンスを初期化
static ConfigDataManager* _instance = nullptr;

// インスタンスの生成&取得
ConfigDataManager* ConfigDataManager::getInstance()
{
    if(!_instance) _instance = new ConfigDataManager();
    return _instance;
}

// インスタンスの消去
void ConfigDataManager::destroy()
{
    delete _instance;
}

// デストラクタ
ConfigDataManager::~ConfigDataManager()
{
    FUNCLOG
    CC_SAFE_RELEASE_NULL(this->debugConfigData);
    CC_SAFE_RELEASE_NULL(this->masterConfigData);
    CC_SAFE_RELEASE_NULL(this->trophyConfigData);
}

// コンストラクタ
ConfigDataManager::ConfigDataManager()
{
    FUNCLOG
    this->debugConfigData = DebugConfigData::create();
    this->masterConfigData = MasterConfigData::create();
    this->trophyConfigData = TrophyConfigData::create();
    
    CC_SAFE_RETAIN(this->debugConfigData);
    CC_SAFE_RETAIN(this->masterConfigData);
    CC_SAFE_RETAIN(this->trophyConfigData);
}

// DebugConfigの取得
DebugConfigData* ConfigDataManager::getDebugConfigData()
{
    return this->debugConfigData;
}

// MasterConfigの取得
MasterConfigData* ConfigDataManager::getMasterConfigData()
{
    return this->masterConfigData;
}

// TrophyConfigの取得
TrophyConfigData* ConfigDataManager::getTrophyConfigData()
{
    return this->trophyConfigData;
}
