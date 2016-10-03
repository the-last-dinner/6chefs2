//
//  PlainArea.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#include "MapObjects/TerrainObject/PlainArea.h"

#include "MapObjects/Terrainstate/TerrainStateCache.h"

// コンストラクタ
PlainArea::PlainArea() { FUNCLOG };

// デストラクタ
PlainArea::~PlainArea() { FUNCLOG };

// 初期化
bool PlainArea::init()
{
    if(!TerrainObject::init()) return false;
    
    return true;
}

TerrainState* PlainArea::getTerrainState(TerrainStateCache* cache) const
{
    return cache->getState(TerrainStateCache::StateType::NORMAL);
}
