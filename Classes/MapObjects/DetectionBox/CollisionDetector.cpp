//
//  CollisionDetector.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/11.
//
//

#include "MapObjects/DetectionBox/CollisionDetector.h"

#include "MapObjects/MapObject.h"

// コンストラクタ
CollisionDetector::CollisionDetector() { FUNCLOG }

// デストラクタ
CollisionDetector::~CollisionDetector()
{
    FUNCLOG
    
    _collistionBoxes.clear();
}

// 初期化
bool CollisionDetector::init()
{
    if(!Node::init()) return false;
    
    return true;
}

CollisionBox* CollisionDetector::getHitCollision(const CollisionBox* collision, const vector<Direction>& directions) const
{
    if(!collision) return nullptr;
    
    for(CollisionBox* other : _collistionBoxes)
    {
        if(collision->isGridHit(other, directions)) return other;
    }
    
    return nullptr;
}

#pragma mark -
#pragma mark Collection

void CollisionDetector::addCollision(CollisionBox* collision)
{
    _collistionBoxes.pushBack(collision);
}

void CollisionDetector::removeCollision(CollisionBox* collision)
{
    _collistionBoxes.eraseObject(collision);
}

#pragma mark -
#pragma mark Detection

bool CollisionDetector::isHit(const CollisionBox* collision) const
{
    if(!collision) return false;
    
    return this->getHitCollision(collision, {});
}

bool CollisionDetector::isHit(const MapObject* mapObject, const vector<Direction>& directions) const
{
    if(!mapObject->getCollision()) return false;
    
    return this->getHitCollision(mapObject->getCollision(), directions);
}

bool CollisionDetector::existsCollisionBetween(const MapObject* obj1, const MapObject* obj2) const
{
    for(CollisionBox* collision : _collistionBoxes)
    {
        if(collision->isBetween(obj1, obj2)) return true;
    }
    
    return false;
}
