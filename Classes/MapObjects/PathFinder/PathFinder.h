//
//  PathFinder.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/07.
//
//

#ifndef __LastSupper__PathFinder__
#define __LastSupper__PathFinder__

#include "Common.h"

class CollisionDetector;
class MapObject;
class MapObjectList;
class PathNode;

class PathFinder : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(PathFinder, MapObjectList*, const Size&)
    
// インスタンス変数
private:
    int _gridWidth { 0 };
    int _gridHeight { 0 };
    MapObjectList* _mapObjectList { nullptr };
    
// インスタンスメソッド
private:
    PathFinder();
    virtual ~PathFinder();
    bool init(MapObjectList* mapObjectList, const Size& mapSize);
    PathNode* find(MapObject* target, PathNode* referenceNode, const Point& destGridPosition, map<Point, PathNode*>& nodeMap);
    
    vector<Point> splitByGrid(const Rect& gridRect);
    PathNode* createNode(const Point& gridPosition);
    PathNode* createNode(const Point& gridPosition, const Point& destGridPosition, PathNode* parent = nullptr);
public:
    deque<Direction> getPath(MapObject* targetMapObject, const Point& destGridPosition);
};

#endif /* defined(__LastSupper__PathFinder__) */
