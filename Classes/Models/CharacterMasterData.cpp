//
//  CharacterMasterData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/16.
//
//

#include "CharacterMasterData.h"

// 初期化
bool CharacterMasterData::init()
{
    string path = FileUtils::getInstance()->fullPathForFilename("csv/character.csv");
    if (path == "") return false;
    this->data = CsvUtils::readCsvFile(path);
    return true;
}

// キャラクター名の取得
string CharacterMasterData::getName(const int chara_id)
{
    return this->data[chara_id][etoi(CharacterMasterData::Keys::NAME)];
}

// キャラクター名とフリガナの組み合わせを取得
string CharacterMasterData::getNameWithRuby(const int chara_id)
{
    return this->getName(chara_id) + " " + this->getRubyWithBracket(chara_id);
}

// キャラクター名のフリガナを取得
string CharacterMasterData::getRuby(const int chara_id)
{
    return this->data[chara_id][etoi(CharacterMasterData::Keys::RUBY)];
}

// キャラクターのフリガナをカッコつきで表示
string CharacterMasterData::getRubyWithBracket(const int chara_id)
{
    string ruby = this->getRuby(chara_id);
    ruby = (ruby != "") ? "(" + ruby + ")" : ""; // フリガナが存在しなければから文字を返す
    return ruby;
}

// キャラクターの通り名を取得
string CharacterMasterData::getStreetName(const int chara_id)
{
    return this->data[chara_id][etoi(CharacterMasterData::Keys::STREET_NAME)];
}

// キャラクターのファイル名を取得
string CharacterMasterData::getFileName(const int chara_id)
{
    return this->data[chara_id][etoi(CharacterMasterData::Keys::FILE_NAME_INI)];
}

// キャラクターの表情差分IDを取得
int CharacterMasterData::getFaceDiff(const int chara_id)
{
    return stoi(this->data[chara_id][etoi(CharacterMasterData::Keys::FACE_DIFF)]);
}

// キャラクターの説明をレベルごとに取得
// @param int level 0 ~ 2
string CharacterMasterData::getDiscription(const int chara_id, const int level)
{
    return this->data[chara_id][level + 1 + etoi(CharacterMasterData::Keys::PROFILE_FLAG)];
}

// キャラクターをメニューに表示するかどうか
bool CharacterMasterData::isDisplay(const int chara_id)
{
    return this->data[chara_id][etoi(CharacterMasterData::Keys::PROFILE_FLAG)] == "1" ? true : false;
}

// 表示するキャラクターリスト
vector<int> CharacterMasterData::getDisplayCharacters()
{
    vector<int> charas;
    for (auto itr:this->data)
    {
        if(this->isDisplay(itr.first))
        {
            charas.push_back(itr.first);
        }
    }
    return charas;
}