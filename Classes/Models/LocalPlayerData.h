//
//  LocalPlayerData.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/04.
//
//

#ifndef LocalPlayerData_h
#define LocalPlayerData_h

#include "define.h"

struct CharacterData;

class LocalPlayerData : public Ref
{
    // Local data member
    static const char* STATUS;
    static const char* TOKEN;
    static const char* SAVE_COUNT;
    static const char* PLAY_TIME;
    static const char* CHAPTER;
    static const char* EQUIPMENT_RIGHT;
    static const char* EQUIPMENT_LEFT;
    static const char* MAP_ID;
    static const char* PARTY;
    static const char* CHARA_ID;
    static const char* OBJ_ID;
    static const char* X;
    static const char* Y;
    static const char* DIRECTION;
    static const char* FRIENDSHIP;
    static const char* EVENT;
    static const char* ITEM;
    static const char* CHARA;
    static const char* BGM;
    
    static const int MAX_COUNT;
    static const int MAX_PLAY_TIME;
    
// Class methods
public:
    CREATE_FUNC_WITH_PARAM(LocalPlayerData, const int)
private:
    LocalPlayerData(){FUNCLOG};
    ~LocalPlayerData(){FUNCLOG};

// Instance valiables
private:
    int local_id {0};
    rapidjson::Document localData {nullptr};
    
public:
    function<void(int)> setTrophy {nullptr};
    
// Instance methods
private:
    bool init(const int local_id);
    // Local data File
    bool loadLocalData(const int local_id);
public:
    void saveLocalData(const int local_id);
    int getLocalId();
    
    // Token
    void setToken();
    string getToken();
    
    // SaveCount
    void incrementSaveCount();
    int getSaveCount();
    
    // PlayTime
    void setPlayTime(const int secound);
    int getPlayTime();
    string getPlayTimeForDisplay();
    
    // Chapter
    void setChapterId(const int chapter_id);
    int getChapterId();
    
    // Friendship
    void setFriendship(const int chara_id, const int level);
    int getFriendship(const int chara_id);
    bool checkFriendship(const int chara_id, const int level);
    int getMaxFriendshipCount();
    
    // Equipment
    void setItemEquipment(const Direction direction, const int item_id);
    int getItemEquipment(const Direction direction);
    bool isEquipedItem(const int item_id);
    
    // Item
    void setItem(const int item_id);
    bool removeItem(const int item_id);
    vector<int> getItemAll();
    bool hasItem(const int item_id);
    
    // Character prifile
    void setCharacterProfile(const int chara_id, const int level);
    int getCharacterProfileLevel(const int chara_id);
    
    // Event
    void setEventNeverAgain(const int map_id, const int event_id, const bool flag = true);
    void setEventStatus(const int map_id, const int event_id, const int status);
    int getEventStatus(const int map_id, const int event_id);
    bool checkEventIsDone(const int map_id, const int event_id);
    bool checkEventStatus(const int map_id, const int event_id, const int status);
    
    // Party
    void setLocation(const vector<CharacterData>& characters);
    void setPartyMember(const CharacterData& chara);
    bool removePartyMember(const int obj_id);
    Location getLocation(const int num = 0);
    CharacterData getPartyMember(const int num = 0);
    vector<CharacterData> getPartyMemberAll();
    
    // BGM
    void setBgm(const string& bgm_name);
    bool removeBgm(const string& bgm_name);
    void removeBgmAll();
    vector<string> getBgmAll();
};

#endif /* LocalPlayerData_h */
