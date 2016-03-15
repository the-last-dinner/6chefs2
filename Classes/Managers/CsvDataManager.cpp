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

const map<CsvDataManager::DataType, string> CsvDataManager::file_type = {
    {CsvDataManager::DataType::CHARACTER, "character"},
    {CsvDataManager::DataType::CHAPTER, "chapter"},
};

#pragma mark Core

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
    CC_SAFE_RELEASE_NULL(this->itemData);
    CC_SAFE_RELEASE_NULL(this->trophyData);
    CC_SAFE_RELEASE_NULL(this->mapData);
}

// コンストラクタ
CsvDataManager::CsvDataManager()
{
    FUNCLOG
    //各CSVデータの取得
    this->itemData = ItemData::create();
    this->trophyData = TrophyData::create();
    this->mapData = MapData::create();
    
    CC_SAFE_RETAIN(this->itemData);
    CC_SAFE_RETAIN(this->trophyData);
    CC_SAFE_RETAIN(this->mapData);
    
    string file_name = "";
    for(auto itr:this->file_type)
    {
        if (itr.first == DataType::CHARACTER)
        {
            for(int i = 1; i <= 3; i++)
            {
                file_name = itr.second + to_string(i);
                map<int, vector<string>> charas;
                charas = CsvUtils::readCsvFile(FileUtils::getInstance()->fullPathForFilename("csv/" + file_name + CSV_EXTENSION));
                for(pair<int, vector<string>> chara : charas)
                {
                    this->csv_data[itr.first][chara.first] = chara.second;
                }
                
            }
        }
        else
        {
            this->csv_data[itr.first] = CsvUtils::readCsvFile(FileUtils::getInstance()->fullPathForFilename("csv/" + itr.second + CSV_EXTENSION));
        }
    }
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

#pragma mark -
#pragma mark Chapter

// チャプター名を取得
string CsvDataManager::getChapterName(const int chapter_id)
{
    return this->csv_data[DataType::CHAPTER][chapter_id][etoi(CsvChapter::NAME)];
}

// チャプターのタイトルを取得
string CsvDataManager::getChapterTitle(const int chapter_id)
{
    return this->csv_data[DataType::CHAPTER][chapter_id][etoi(CsvChapter::TITLE)];
}

#pragma mark -
#pragma mark Character

// キャラクター名の取得
string CsvDataManager::getCharaName(const int chara_id)
{
    return this->csv_data[DataType::CHARACTER][chara_id][etoi(CsvCharacter::NAME)];
}

// キャラクター名とフリガナの組み合わせを取得
string CsvDataManager::getCharaNameWithRuby(const int chara_id)
{
    return this->getCharaName(chara_id) + " " + this->getCharaRubyWithBracket(chara_id);
}

// キャラクター名のフリガナを取得
string CsvDataManager::getCharaRuby(const int chara_id)
{
    return this->csv_data[DataType::CHARACTER][chara_id][etoi(CsvCharacter::RUBY)];
}

// キャラクターのフリガナをカッコつきで表示
string CsvDataManager::getCharaRubyWithBracket(const int chara_id)
{
    string ruby = this->getCharaRuby(chara_id);
    ruby = (ruby != "") ? "(" + ruby + ")" : ""; // フリガナが存在しなければから文字を返す
    return ruby;
}

// キャラクターの通り名を取得
string CsvDataManager::getCharaStreetName(const int chara_id)
{
    return this->csv_data[DataType::CHARACTER][chara_id][etoi(CsvCharacter::STREET_NAME)];
}

// キャラクターのファイル名を取得
string CsvDataManager::getCharaFileName(const int chara_id)
{
    return this->csv_data[DataType::CHARACTER][chara_id][etoi(CsvCharacter::FILE_NAME_INI)];
}

// キャラクターの表情差分IDを取得
int CsvDataManager::getCharaFaceDiff(const int chara_id)
{
    return stoi(this->csv_data[DataType::CHARACTER][chara_id][etoi(CsvCharacter::FACE_DIFF)]);
}

// キャラクターの説明をレベルごとに取得
// @param int level 0 ~ 2
string CsvDataManager::getCharaDiscription(const int chara_id, const int level)
{
    return this->csv_data[DataType::CHARACTER][chara_id][level + 1 + etoi(CsvCharacter::PROFILE_FLAG)];
}

// キャラクターをメニューに表示するかどうか
bool CsvDataManager::isDisplayChara(const int chara_id)
{
    return this->csv_data[DataType::CHARACTER][chara_id][etoi(CsvCharacter::PROFILE_FLAG)] == "1" ? true : false;
}

// 表示するキャラクターリスト
vector<int> CsvDataManager::getDisplayCharacters()
{
    vector<int> charas;
    for (auto itr:this->csv_data[DataType::CHARACTER])
    {
        if(this->isDisplayChara(itr.first))
        {
            charas.push_back(itr.first);
        }
    }
    return charas;
}