//
//  LadderArea.cpp
//  6chefs2
//
//  Created by S.inami on 2016/12/16.
//
//

#include "MapObjects/TerrainObject/LadderArea.h"

#include "MapObjects/Terrainstate/TerrainStateCache.h"

// コンストラクタ
LadderArea::LadderArea() { FUNCLOG };

// デストラクタ
LadderArea::~LadderArea() { FUNCLOG };

// 初期化
bool LadderArea::init()
{
    if(!TerrainObject::init()) return false;
    
    return true;
}

TerrainState* LadderArea::getTerrainState(TerrainStateCache* cache) const
{
    return cache->getState(TerrainStateCache::StateType::LADDER);
}
