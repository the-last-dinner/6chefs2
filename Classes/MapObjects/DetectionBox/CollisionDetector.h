//
//  CollisionDetector.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/11.
//
//

#ifndef CollisionDetector_h
#define CollisionDetector_h

#include "MapObjects/DetectionBox/CollisionBox.h"

class MapObject;

class CollisionDetector : public Node
{
// クラスメソッド
public:
    CREATE_FUNC(CollisionDetector)

// インスタンス変数
private:
    Vector<CollisionBox*> _collisionBoxes {};
    
// インスタンスメソッド
private:
    CollisionDetector();
    virtual ~CollisionDetector();
    virtual bool init() override;
    CollisionBox* getIntersectsCollision(const CollisionBox* collision, const vector<Direction>& directions) const;
    CollisionBox* getIntersectsCollisionIncludeIgnorable(const CollisionBox* collision, const vector<Direction>& directions) const;
    CollisionBox* getIntersectsCollision(const CollisionBox* collision, const Point& gridPosition) const;
    CollisionBox* getIntersectsCollisionIncludeIgnorable(const CollisionBox* collision, const Point& gridPosition) const;
    CollisionBox* getIntersectsCollisionForPath(const CollisionBox* collision, const Point& gridPosition) const;
public:
    void addCollision(CollisionBox* collision);
    void removeCollision(CollisionBox* collision);
public:
    bool intersects(const CollisionBox* collision, const Point& gridPosition = Point::ZERO) const;
    bool intersectsForPath(const CollisionBox* collision, const Point& gridPosition = Point::ZERO) const;
    bool intersects(const MapObject* mapObject, const vector<Direction>& directions) const;
    bool intersectsExceptIgnorable(const CollisionBox* collision, const Point& gridPosition = Point::ZERO) const;
    bool intersectsExceptIgnorable(const MapObject* mapObject, const vector<Direction>& directions) const;
    bool existsCollisionBetween(const MapObject* obj1, const MapObject* obj2) const;
};

#endif /* CollisionDetector_h */
