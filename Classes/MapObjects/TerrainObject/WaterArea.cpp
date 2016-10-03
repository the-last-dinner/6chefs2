//
//  WaterArea.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#include "MapObjects/TerrainObject/WaterArea.h"

#include "MapObjects/Terrainstate/TerrainStateCache.h"

// コンストラクタ
WaterArea::WaterArea() { FUNCLOG };

// デストラクタ
WaterArea::~WaterArea() { FUNCLOG };

// 初期化
bool WaterArea::init()
{
    if(!TerrainObject::init()) return false;
    
    return true;
}

TerrainState* WaterArea::getTerrainState(TerrainStateCache* cache) const
{
    return cache->getState(TerrainStateCache::StateType::SWIM);
}
