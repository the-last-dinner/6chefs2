//
//  TerrainState.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/08/31.
//
//

#include "MapObjects/TerrainState/TerrainState.h"

#include "CocosStudio/CSNode.h"

#include "MapObjects/Character.h"

// コンストラクタ
TerrainState::TerrainState() {}

// デストラクタ
TerrainState::~TerrainState() {}

// 初期化
bool TerrainState::init()
{
    return true;
}

bool TerrainState::isRunnable() const
{
    return _isRunnable;
}

bool TerrainState::consumeStaminaWalking() const
{
    return _consumeStaminaWalking;
}

float TerrainState::getStaminaConsumptionRatio() const
{
    return _staminaConsumptionRatio;
}

bool TerrainState::isTrunable(const Direction& direction) const
{
    
    if(_directionIntToturnable.count(direction.getInt()) == 0) return true;
    
    return _directionIntToturnable.at(direction.getInt());
}
