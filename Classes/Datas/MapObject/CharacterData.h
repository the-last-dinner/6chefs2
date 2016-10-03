//
//  CharacterData.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/18.
//
//

#ifndef CharacterData_h
#define CharacterData_h

#include "define.h"
#include "Managers/CsvDataManager.h"

struct CharacterData
{
    // プロパティ
    int chara_id {static_cast<int>(CharacterID::UNDIFINED)};
    int obj_id {static_cast<int>(ObjectID::UNDIFINED)};
    Location location {};
    CharacterMovePattern move_pattern {CharacterMovePattern::NONE};
    
    // コンストラクタ
    CharacterData(){};
    CharacterData(const int chara_id, const int obj_id, const Location& location):chara_id(chara_id), obj_id(obj_id), location(location){};
    
    // ドット画像のファイル名
    string getDotFileName() const { return CsvDataManager::getInstance()->getCharacterData()->getFileName(this->chara_id) + "_0_0.png"; };
    
    // 立ち絵画像のファイル名
    string getStandFileName() const { return CsvDataManager::getInstance()->getCharacterData()->getFileName(this->chara_id) + "_s_1.png"; }
    
    // csbファイルパス
    string getCsbFilePath() const { return "character/" + CsvDataManager::getInstance()->getCharacterData()->getFileName(this->chara_id) + ".csb"; }
};

#endif /* CharacterData_h */
