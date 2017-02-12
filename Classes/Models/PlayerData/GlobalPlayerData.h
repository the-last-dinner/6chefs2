//
//  GlobalPlayerData.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/04.
//
//

#ifndef GlobalPlayerData_h
#define GlobalPlayerData_h

#include "define.h"
#include "Managers/KeyconfigManager.h"
#include "Models/ConfigData/TrophyConfigData.h"

class GlobalPlayerData : public Ref
{
public:
    // path
    static const string GLOBAL_DATA_PATH;
    static const string GLOBAL_TEMPLATE_PATH;
    
    // Save data member
    static const char* STATUS;
    static const char* GLOBAL_ID;
    static const char* CLEAR_COUNT;
    static const char* BEST_SAVE_COUNT;
    static const char* BEST_CLEAR_TIME;
    static const char* TROPHY;
    static const char* TOKENS;
    static const char* BGM_VOLUME;
    static const char* SE_VOLUME;
    static const char* VOICE_VOLUME;
    static const char* CURSOR_KEY;
    static const char* ENTER_KEY;
    static const char* DASH_KEY;
    
public:
    CREATE_FUNC(GlobalPlayerData)
private:
    GlobalPlayerData(){FUNCLOG}
    ~GlobalPlayerData(){FUNCLOG}
// Instance valiables
private:
    rapidjson::Document globalData {nullptr};
    TrophyConfigData* trophyConfigData {nullptr};

// Instance methods
private:
    bool init();
    bool loadGlobalData();
    
public:
    bool initGlobalData();
    void saveGlobalData();
    
    // status
    int getStatus();
    
    // Clear count
    void setClearCount(const string& token);
    int getClearCount();
    bool isCleared();
    
    // Minimum save count
    void setBestSaveCount(const int save_count);
    int getBestSaveCount();
    
    // Best clear time
    void setBestClearTime(const int play_time);
    int getBestClearTimeSecound();
    
    // Trophy data
    void setTrophy(const int trophy_id);
    bool hasTrophy(const int trophy_id);
    void setTrophyComplete();
    
    // Clear token
    bool isClearedToken(const string& token);
    void setClearToken(const string& token);
    
    // Volume
    void setBgmVolume(const float& volume);
    float getBgmVolume();
    void setSeVolume(const float& volume);
    float getSeVolume();
    void setVoiceVolume(const float& volume);
    float getVoiceVolume();
    
    // KeyConfig
    void setCursorKey(const KeyconfigManager::CursorKeyType keyType);
    KeyconfigManager::CursorKeyType getCursorKey();
    void setDashKey(const KeyconfigManager::DashKeyType keyType);
    KeyconfigManager::DashKeyType getDashKey();
    void saveKeyConfig(const KeyconfigManager::CursorKeyType cursorKey, const KeyconfigManager::DashKeyType dashKey);
    
};

#endif /* GlobalPlayerData_h */
