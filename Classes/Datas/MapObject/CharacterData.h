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
    string getDotFileName(){return CsvDataManager::getInstance()->getCharaFileName(this->chara_id) + "_0_0.png";};
    
    // 立ち絵画像のファイル名
    string getStandFileName(){return CsvDataManager::getInstance()->getCharaFileName(this->chara_id) + "_s_1.png";}
};

#endif /* CharacterData_h */
