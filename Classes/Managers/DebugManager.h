//
//  DebugManager.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/06.
//
//

#ifndef DebugManager_h
#define DebugManager_h

#include "define.h"

class DebugManager
{
    // singleton用関数
public:
    static DebugManager* getInstance();
    static void destroy();
private:
    DebugManager(); // コンストラクタ
    ~DebugManager(); // デストラクタ
    DebugManager(const DebugManager& other){}; // コピーコンストラクタ
    DebugManager& operator = (const DebugManager& other); // 代入演算子

    // 定数
private:
    static const char* DEBUG_MASK;
    static const char* STATS;
    static const char* INVINCIBLE_MODE;
    static const char* PLAIN_DATA;
    static const char* CRYPT_TRIGGER;
    
    // インスタンス変数
private:
    bool hasDebugConfig {false};
    rapidjson::Document debugConfig {nullptr};
    string file_path {};
    
    // インスタンスメソッド
public:
    bool displayDebugMask();
    bool displayStats();
    bool isInvincibleMode();
    bool isPlainData();
    void setOffPlainData();
    bool getCryptTrigger();
    void setOffCryptTrigger();
private:
    void writeConfig();
};

#endif /* DebugManager_h */
