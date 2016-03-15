//
//  CsvDataManager.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/09/13.
//
//

#include "Managers/CsvDataManager.h"
#include "Utils/StringUtils.h"

// 唯一のインスタンスを初期化
static CsvDataManager* _instance = nullptr;

// インスタンスの生成&取得
CsvDataManager* CsvDataManager::getInstance()
{
    if(!_instance) _instance = new CsvDataManager();
    return _instance;
}

// インスタンスの消去
void CsvDataManager::destroy()
{
    delete _instance;
}

// デストラクタ
CsvDataManager::~CsvDataManager()
{
    FUNCLOG
    
    // 参照カウントダウン
    CC_SAFE_RELEASE_NULL(this->itemData);
    CC_SAFE_RELEASE_NULL(this->trophyData);
    CC_SAFE_RELEASE_NULL(this->mapData);
    CC_SAFE_RELEASE_NULL(this->chapterData);
    CC_SAFE_RELEASE_NULL(this->characterData);
}

// コンストラクタ
CsvDataManager::CsvDataManager()
{
    FUNCLOG
    
    // 各CSVデータの取得
    this->itemData = ItemData::create();
    this->trophyData = TrophyData::create();
    this->mapData = MapData::create();
    this->chapterData = ChapterData::create();
    this->characterData = CharacterMasterData::create();
    
    // 参照カウントアップ
    CC_SAFE_RETAIN(this->itemData);
    CC_SAFE_RETAIN(this->trophyData);
    CC_SAFE_RETAIN(this->mapData);
    CC_SAFE_RETAIN(this->chapterData);
    CC_SAFE_RETAIN(this->characterData);
}

// アイテムデータインスタンスの取得
ItemData* CsvDataManager::getItemData()
{
    return this->itemData;
}

// トロフィーデータインスタンスの取得
TrophyData* CsvDataManager::getTrophyData()
{
    return this->trophyData;
}

// マップデータインスタンスの取得
MapData* CsvDataManager::getMapData()
{
    return this->mapData;
}

// チャプターデータインスタンスの取得
ChapterData* CsvDataManager::getChapterData()
{
    return this->chapterData;
}

// キャラクターデータインスタンスの取得
CharacterMasterData* CsvDataManager::getCharacterData()
{
    return this->characterData;
}