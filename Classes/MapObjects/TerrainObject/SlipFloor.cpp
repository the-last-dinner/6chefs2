//
//  SlipFloor.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#include "MapObjects/TerrainObject/SlipFloor.h"

#include "MapObjects/Terrainstate/TerrainStateCache.h"

// コンストラクタ
SlipFloor::SlipFloor() { FUNCLOG };

// デストラクタ
SlipFloor::~SlipFloor() { FUNCLOG };

// 初期化
bool SlipFloor::init()
{
    if(!TerrainObject::init()) return false;
    
    return true;
}

TerrainState* SlipFloor::getTerrainState(TerrainStateCache* cache) const
{
    return cache->getState(TerrainStateCache::StateType::SLIP);
}
