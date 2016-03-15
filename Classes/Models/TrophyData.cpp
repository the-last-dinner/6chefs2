//
//  TrophyData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/16.
//
//

#include "TrophyData.h"

// 初期化
bool TrophyData::init()
{
    string path = FileUtils::getInstance()->fullPathForFilename("csv/trophy.csv");
    if (path == "") return false;
    this->data = CsvUtils::readCsvFile(path);
    return true;
}

// トロフィーの名前を取得
string TrophyData::getName(const int trophy_id)
{
    return this->data[trophy_id][etoi(TrophyData::Keys::NAME)];
}

// トロフィーの条件を取得
string TrophyData::getCondition(const int trophy_id)
{
    return this->data[trophy_id][etoi(TrophyData::Keys::CONDITION)];
}

// トロフィーのコメントを取得
string TrophyData::getComment(const int trophy_id)
{
    return this->data[trophy_id][etoi(TrophyData::Keys::COMMENT)];
}

// トロフィーを全て取得
vector<int> TrophyData::getIdAll()
{
    vector<int> trophies;
    for (auto itr:this->data)
    {
        trophies.push_back(itr.first);
    }
    return trophies;
}