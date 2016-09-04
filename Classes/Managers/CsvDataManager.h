//
//  CsvDataManager.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/09/13.
//
//

#ifndef LastSupper_CsvDataManager_h
#define LastSupper_CsvDataManager_h

#include "define.h"
#include "Models/CsvData/ItemData.h"
#include "Models/CsvData/TrophyData.h"
#include "Models/CsvData/MapData.h"
#include "Models/CsvData/ChapterData.h"
#include "Models/CsvData/CharacterMasterData.h"
#include "Models/CsvData/DocumentData.h"

class CsvDataManager
{
    
// インスタンス変数
private:
    ItemData* itemData {nullptr};
    TrophyData* trophyData {nullptr};
    MapData* mapData {nullptr};
    ChapterData* chapterData {nullptr};
    CharacterMasterData* characterData {nullptr};
    DocumentData* documentData {nullptr};
    
// インスタンスメソッド
public:
    ItemData* getItemData();
    TrophyData* getTrophyData();
    MapData* getMapData();
    ChapterData* getChapterData();
    CharacterMasterData* getCharacterData();
    DocumentData* getDocumentData();
    
// singleton用関数
public:
    static CsvDataManager* getInstance();
    static void destroy();
private:
    CsvDataManager(); // コンストラクタ
    ~CsvDataManager(); // デストラクタ
    CsvDataManager(const CsvDataManager& other){}; // コピーコンストラクタ
    CsvDataManager& operator = (const CsvDataManager& other); // 代入演算子
};
#endif