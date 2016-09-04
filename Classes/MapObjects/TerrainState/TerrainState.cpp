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
TerrainState::TerrainState() { FUNCLOG }

// デストラクタ
TerrainState::~TerrainState() { FUNCLOG }

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
