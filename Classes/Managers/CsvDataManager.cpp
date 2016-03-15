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
    {CsvDataManager::DataType::MAP, "map"},
    {CsvDataManager::DataType::ITEM, "item"},
    {CsvDataManager::DataType::CHARACTER, "character"},
    {CsvDataManager::DataType::CHAPTER, "chapter"},
    {CsvDataManager::DataType::TROPHY, "trophy"},
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
{FUNCLOG}

// コンストラクタ
CsvDataManager::CsvDataManager()
{
    FUNCLOG
    //各CSVデータの取得
    string file_name = "";
    for(auto itr:this->file_type)
    {
        if (itr.first == DataType::CHARACTER)
        {
            for(int i = 1; i <= 3; i++)
            {
                file_name = itr.second + to_string(i);
                map<int, vector<string>> charas;
                charas = this->readCsvFile(file_name);
                for(pair<int, vector<string>> chara : charas)
                {
                    this->CsvDataManager::csv_data[itr.first][chara.first] = chara.second;
                }
                
            }
        }
        else
        {
            this->csv_data[itr.first] = this->readCsvFile(itr.second);
        }
    }
}

//CSV読み取り
map<int, vector<string>> CsvDataManager::readCsvFile(string file_name)
{
    //ファイル読み込み
    ifstream file(FileUtils::getInstance()->fullPathForFilename("csv/" + file_name + CSV_EXTENSION));
    map<int, vector<string>> values;
    string str;
    int p, i, data_id;
    //ファイル読み込み失敗時
    if(file.fail()){
        CCLOG("Reading csv file of %s is failed.", file_name.c_str());
        return values;
    }
    //csvデータ格納
    while(getline(file, str)){
        // 複合化
        LastSupper::StringUtils::encryptXor(str);
        
        //コメント箇所は除く
        if( (p = str.find("//")) != str.npos ) continue;
        vector<string> inner;
        i = 0;
        //カンマがあるかを探し、そこまでをvaluesに格納
        while( (p = str.find(",")) != str.npos ){
            if (i == 0)
            {
                data_id = stoi(str.substr(0, p));
            }
            inner.push_back(str.substr(0, p));
            //strの中身は","の1文字を飛ばす
            str = str.substr(p+1);
            i++;
        }
        inner.push_back(str);
        values[data_id] = inner;
    }
    return values;
}

#pragma mark -
#pragma mark Map

// マップ名を取得
string CsvDataManager::getMapName(const int map_id)
{
    return this->csv_data[DataType::MAP][map_id][etoi(CsvMap::NAME)];
}

// マップのファイル名を取得
string CsvDataManager::getMapFileName(const int map_id)
{
    return this->csv_data[DataType::MAP][map_id][etoi(CsvMap::FILE_NAME)];
}

// マップのファイル名を全取得
vector<string> CsvDataManager::getMapFileNameAll()
{
    vector<string> fileNames {};
    for (auto itr:this->csv_data[DataType::MAP])
    {
        fileNames.push_back(itr.second[etoi(CsvMap::FILE_NAME)]);
    }
    return fileNames;
}

#pragma mark -
#pragma mark Item

// アイテム名を取得
string CsvDataManager::getItemName(const int item_id)
{
    return (item_id >= 0) ? this->csv_data[DataType::ITEM][item_id][etoi(CsvItem::NAME)] : "アイテムがありません";
}

// アイテムの説明を取得
string CsvDataManager::getItemDiscription(const int item_id)
{
    return (item_id >= 0) ? this->csv_data[DataType::ITEM][item_id][etoi(CsvItem::DISCRIPTION)] : "アイテムがありません";
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

#pragma mark -
#pragma mark Trophy

// トロフィーの名前を取得
string CsvDataManager::getTrophyName(const int trophy_id)
{
    return this->csv_data[DataType::TROPHY][trophy_id][etoi(CsvTrophy::NAME)];
}

// トロフィーの条件を取得
string CsvDataManager::getTrophyCondition(const int trophy_id)
{
    return this->csv_data[DataType::TROPHY][trophy_id][etoi(CsvTrophy::CONDITION)];
}

// トロフィーのコメントを取得
string CsvDataManager::getTrophyComment(const int trophy_id)
{
    return this->csv_data[DataType::TROPHY][trophy_id][etoi(CsvTrophy::COMMENT)];
}

// トロフィーを全て取得
vector<int> CsvDataManager::getTrophyIdAll()
{
    vector<int> trophies;
    for (auto itr:this->csv_data[DataType::TROPHY])
    {
        trophies.push_back(itr.first);
    }
    return trophies;
}