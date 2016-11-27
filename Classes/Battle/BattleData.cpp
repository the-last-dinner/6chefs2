//
//  BattleData.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#include "Battle/BattleData.h"

// コンストラクタ
BattleData::BattleData() { FUNCLOG }

// デストラクタ
BattleData::~BattleData() { FUNCLOG }

// 初期化
bool BattleData::init()
{
    return true;
}

#pragma mark -
#pragma mark TargetObjectIds
void BattleData::setTargetObjectIds(const vector<int>& ids)
{
    _targetObjectIds = ids;
}

vector<int> BattleData::getTargetObjectIds() const
{
    return _targetObjectIds;
}
