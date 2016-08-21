//
//  MasterConfigManager.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/18.
//
//

#ifndef MasterConfigManager_h
#define MasterConfigManager_h

#include "define.h"

class MasterConfigManager
{
    // singleton用関数
public:
    static MasterConfigManager* getInstance();
    static void destroy();
private:
    MasterConfigManager(); // コンストラクタ
    ~MasterConfigManager(); // デストラクタ
    MasterConfigManager(const MasterConfigManager& other){}; // コピーコンストラクタ
    MasterConfigManager& operator = (const MasterConfigManager& other); // 代入演算子
    
    // 定数
public:
    static const char* DISPLAY;
    static const char* FRIENDSHIP;
    static const char* SPECIAL_ROOM;
    
    // インスタンス変数
private:
    rapidjson::Document masterConfig {nullptr};
    string file_path {};
    
    // インスタンスメソッド
public:
    bool isDisplay(const char* targetProperty);
};

#endif /* MasterConfigManager_hpp */
