//
//  ChangeMapHistory.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/27.
//
//

#include "Models/ChangeMapHistory.h"

// コンストラクタ
ChangeMapHistory::ChangeMapHistory() {};

// デストラクタ
ChangeMapHistory::~ChangeMapHistory() {};

// 最新の履歴を取得
Relation ChangeMapHistory::getLatestRelation() const
{
    return this->relations.back();
}

// 最古の履歴を取得
Relation ChangeMapHistory::getOldestRelation() const
{
    return this->relations.front();
}

// 履歴を追加
void ChangeMapHistory::add(const Relation& relation)
{
    this->relations.push_back(relation);
}

// 最新の履歴を削除
void ChangeMapHistory::deleteLatest()
{
    this->relations.pop_back();
}

// 最古の履歴を削除
void ChangeMapHistory::deleteOldest()
{
    this->relations.pop_front();
}