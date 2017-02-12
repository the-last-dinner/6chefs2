//
//  ConfigDataManager.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/18.
//
//

#ifndef ConfigDataManager_h
#define ConfigDataManager_h

#include "define.h"

#include "Models/ConfigData/DebugConfigData.h"
#include "Models/ConfigData/MasterConfigData.h"
#include "Models/ConfigData/TrophyConfigData.h"

class ConfigDataManager
{
    // singleton用関数
public:
    static ConfigDataManager* getInstance();
    static void destroy();
private:
    ConfigDataManager(); // コンストラクタ
    ~ConfigDataManager(); // デストラクタ
    ConfigDataManager(const ConfigDataManager& other){}; // コピーコンストラクタ
    ConfigDataManager& operator = (const ConfigDataManager& other); // 代入演算子
    
    // インスタンス変数
private:
    DebugConfigData* debugConfigData {};
    MasterConfigData* masterConfigData {};
    TrophyConfigData* trophyConfigData {};
    
    // インスタンスメソッド
public:
    DebugConfigData* getDebugConfigData();
    MasterConfigData* getMasterConfigData();
    TrophyConfigData* getTrophyConfigData();
};

#endif /* ConfigDataManager_hpp */
