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
    _collisionBoxes.clear();
}

// 初期化
bool CollisionDetector::init()
{
    if (!Node::init()) return false;
    
    return true;
}

CollisionBox* CollisionDetector::getIntersectsCollision(const CollisionBox* collision, const Point& gridPosition) const
{
    if (!collision) return nullptr;
    
    for (CollisionBox* other : _collisionBoxes) {
        // 親のMapObjectが動いていたら無視
        if (other->getMOParent()->isMoving()) continue;
        if (collision->intersectsGrid(other, gridPosition)) return other;
    }
    
    return nullptr;
}

CollisionBox* CollisionDetector::getIntersectsCollisionIncludeIgnorable(const CollisionBox* collision, const Point& gridPosition) const
{
    if (!collision) return nullptr;
    
    for (CollisionBox* other : _collisionBoxes) {
        if (collision->intersectsGrid(other, gridPosition)) return other;
    }
    
    return nullptr;
}

CollisionBox* CollisionDetector::getIntersectsCollision(const CollisionBox* collision, const vector<Direction>& directions) const
{
    if (!collision) return nullptr;
    
    for (CollisionBox* other : _collisionBoxes) {
        // 親のMapObjectが動いていたら無視
        if (other->getMOParent()->isMoving()) continue;
        if (collision->intersectsGrid(other, directions)) return other;
    }
    
    return nullptr;
}

CollisionBox* CollisionDetector::getIntersectsCollisionIncludeIgnorable(const CollisionBox* collision, const vector<Direction>& directions) const
{
    if (!collision) return nullptr;
    
    for (CollisionBox* other : _collisionBoxes) {
        if (collision->intersectsGrid(other, directions)) return other;
    }
    
    return nullptr;
}

CollisionBox* CollisionDetector::getIntersectsCollisionForPath(const CollisionBox* collision, const Point& gridPosition) const
{
    if (!collision) return nullptr;
    
    for (CollisionBox* other : _collisionBoxes) {
        if (collision->intersectsGridForPath(other, gridPosition)) return other;
    }
    
    return nullptr;
}

#pragma mark -
#pragma mark Collection

void CollisionDetector::addCollision(CollisionBox* collision)
{
    if (!collision) return;
    
    _collisionBoxes.pushBack(collision);
}

void CollisionDetector::removeCollision(CollisionBox* collision)
{
    if (!collision) return;
    
    _collisionBoxes.eraseObject(collision);
}

#pragma mark -
#pragma mark Detection

bool CollisionDetector::intersects(const CollisionBox* collision, const Point& gridPosition) const
{
    if (!collision) return false;
    
    return this->getIntersectsCollisionIncludeIgnorable(collision, gridPosition);
}

bool CollisionDetector::intersectsForPath(const CollisionBox* collision, const Point& gridPosition) const
{
    if (!collision) return false;
    
    return this->getIntersectsCollisionForPath(collision, gridPosition);
}

bool CollisionDetector::intersects(const MapObject* mapObject, const vector<Direction>& directions) const
{
    if (!mapObject->getCollision()) return false;
    
    return this->getIntersectsCollisionIncludeIgnorable(mapObject->getCollision(), directions);
}

bool CollisionDetector::intersectsExceptIgnorable(const CollisionBox* collision, const Point& gridPosition) const
{
    if (!collision) return false;
    
    return this->getIntersectsCollision(collision, gridPosition);
}

bool CollisionDetector::intersectsExceptIgnorable(const MapObject* mapObject, const vector<Direction>& directions) const
{
    if (!mapObject->getCollision()) return false;
    
    return this->getIntersectsCollision(mapObject->getCollision(), directions);
}

bool CollisionDetector::existsCollisionBetween(const MapObject* obj1, const MapObject* obj2) const
{
    for (CollisionBox* collision : _collisionBoxes) {
        if (collision->isBetween(obj1->getCollision(), obj2->getCollision())) return true;
    }
    
    return false;
}
