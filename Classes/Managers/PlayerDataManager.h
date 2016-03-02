//
//  PlayerDataManager.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/06/28.
//
//

#ifndef __LastSupper__PlayerDataManager__
#define __LastSupper__PlayerDataManager__

#include "define.h"

class StopWatch;
class GlobalPlayerData;
class LocalPlayerData;

class PlayerDataManager
{

// インスタンス変数
private:
    GlobalPlayerData* globalData {nullptr};
    LocalPlayerData* localData {nullptr};
    StopWatch* timer {nullptr};

// インスタンスメソッド
public:
    void initLocalData();
    void setGameClear(const int end_id);
    void setGameStart(const int local_id);
    void save(const int local_id);
    void cryptSaveData();
    
    GlobalPlayerData* getGlobalData() const;
    LocalPlayerData* getLocalData() const;
    string getCurrentTimeForDisplay();

// singleton用関数
public:
    static PlayerDataManager* getInstance();
    static void  destroy();
private:
    PlayerDataManager();    //コンストラクタ
    ~PlayerDataManager();   //デストラクタ
    PlayerDataManager(const PlayerDataManager& other){};              //コピーコンストラクタ
    PlayerDataManager& operator = (const PlayerDataManager& other); //代入演算子
    
};
#endif /* defined(__LastSupper__PlayerDataManager__) */
