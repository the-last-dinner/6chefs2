//
//  TerrainObject.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#include "MapObjects/TerrainObject/TerrainObject.h"

#include "MapObjects/Character.h"
#include "MapObjects/TerrainState/TerrainState.h"

// コンストラクタ
TerrainObject::TerrainObject() {};

// デストラクタ
TerrainObject::~TerrainObject() {};

// 初期化
bool TerrainObject::init()
{
    if(!MapObject::init()) return false;
    
    return true;
}
