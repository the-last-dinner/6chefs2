//
//  GhostObject.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#include "MapObjects/GhostObject.h"

// コンストラクタ
GhostObject::GhostObject() { FUNCLOG }

// デストラクタ
GhostObject::~GhostObject() { FUNCLOG }

// 初期化
bool GhostObject::init()
{
    if(!MapObject::init()) return false;
    
    return true;
}

// 当たり判定は常にfalseを返す
bool GhostObject::isHit(const vector<Direction>& directions, bool ignoreCollision) const
{
    return false;
}

bool GhostObject::isHit(const MapObject* other) const
{
    return false;
}
