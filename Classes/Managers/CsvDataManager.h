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

class CsvDataManager
{
public:
    enum struct DataType
    {
        MAP,
        ITEM,
        CHARACTER,
        CHAPTER,
        TROPHY
    };
    enum struct CsvMap
    {
        ID,
        NAME,
        FILE_NAME
    };
    enum struct CsvChapter
    {
        ID,
        NAME,
        TITLE
    };
    enum struct CsvItem
    {
        ID,
        NAME,
        DISCRIPTION,
    };
    enum struct CsvCharacter
    {
        ID,
        NAME,
        RUBY,
        STREET_NAME,
        FILE_NAME_INI,
        FACE_DIFF,
        PROFILE_FLAG,
        PROFILE0,
        PROFILE1,
        PROFILE2
    };
    enum struct CsvTrophy
    {
        ID,
        NAME,
        CONDITION,
        COMMENT
    };
    
// インスタンス変数
private:
    static const map<DataType, string> file_type;
    map<DataType, map<int, vector<string>>> csv_data;
    
// singleton用関数
public:
    static CsvDataManager* getInstance();
    static void destroy();
private:
    CsvDataManager(); // コンストラクタ
    ~CsvDataManager(); // デストラクタ
    CsvDataManager(const CsvDataManager& other){}; // コピーコンストラクタ
    CsvDataManager& operator = (const CsvDataManager& other); // 代入演算子
    
// 関数
private:
    map<int, vector<string>> readCsvFile(string file_name);
    
public:
    // map
    string getMapName(const int map_id);
    string getMapFileName(const int map_id);
    vector<string> getMapFileNameAll();
    
    // item
    string getItemName(const int item_id);
    string getItemDiscription(const int item_id);
    
    // chapter
    string getChapterName(const int chapter_id);
    string getChapterTitle(const int chapter_id);
    
    // character
    string getCharaName(const int chara_id);
    string getCharaNameWithRuby(const int chara_id);
    string getCharaRuby(const int chara_id);
    string getCharaRubyWithBracket(const int chara_id);
    string getCharaStreetName(const int chara_id);
    string getCharaFileName(const int chara_id);
    int getCharaFaceDiff(const int chara_id);
    string getCharaDiscription(const int chara_id, const int level);
    bool isDisplayChara(const int chara_id);
    vector<int> getDisplayCharacters();
    
    // trophy
    string getTrophyName(const int trophy_id);
    string getTrophyCondition(const int trophy_id);
    string getTrophyComment(const int trophy_id);
    vector<int> getTrophyIdAll();
};
#endif