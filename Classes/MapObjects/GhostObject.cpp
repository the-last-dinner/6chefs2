//
//  GhostObject.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#include "MapObjects/GhostObject.h"

// コンストラクタ
GhostObject::GhostObject() {FUNCLOG};

// デストラクタ
GhostObject::~GhostObject() {FUNCLOG};

// 初期化
bool GhostObject::init()
{
    if(!MapObject::init()) return false;
    
    // 当たり判定オフ
    this->setHit(false);
    
    return true;
}

// 周りには当たり判定オブジェクトがないとする
vector<Rect> GhostObject::getWorldGridCollisionRects()
{
    return {};
}

// 当たり判定は常にfalseを返す
const bool GhostObject::isHit(const vector<Direction>& directions) const { return false; }
