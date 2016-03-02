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

class MapObjectList;

class PathFinder : public Ref
{
// クラス宣言
private:
    class PathNode;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(PathFinder, const Size&)
    
// インスタンス変数
private:
    int gridWidth { 0 };
    int gridHeight { 0 };
    
// インスタンスメソッド
private:
    PathFinder();
    ~PathFinder();
    bool init(const Size& mapSize);
    PathNode* find(PathNode* referenceNode, const Point& destGridPosition, map<Point, PathNode*>& nodeMap);
    
    vector<Point> splitByGrid(const Rect& gridRect);
    PathNode* createNode(const Point& gridPosition);
    PathNode* createNode(const Point& gridPosition, const Point& destGridPosition, PathNode* parent = nullptr);
public:
    deque<Direction> getPath(const Rect& chaserGridRect, const vector<Rect>& collisionGridRects, const Point& destGridPosition);
    
// クラス
private:
    class PathNode : public Ref
    {
    public:
        enum struct State
        {
            OPEN,
            CLOSED,
            NONE,
            CANT,
        };
    
    public:
        CREATE_FUNC_WITH_PARAM(PathNode, const Point&)

    private:
        Point _gridPoint {Point::ZERO};
        State _state {State::NONE};
        int _actualCost { 0 };
        int _estimatedCost { 0 };
        PathNode* _parent { nullptr };
        
    private:
        bool init(const Point& gridPoint) { _gridPoint = gridPoint; return true; };
    public:
        void setState(const State state) {_state = state;};
        void setActualCost(const int cost) {_actualCost = cost;};
        void setEstimatedCost(const int cost) {_estimatedCost = cost;};
        void setParent(PathNode* node) {_parent = node;};
        
        Point getGridPoint() const {return _gridPoint;};
        State getState() const {return _state;};
        int getActualCost() const {return _actualCost;};
        int getEstimatedCost() const {return _estimatedCost;};
        int getScore() const { return _actualCost + _estimatedCost; };
        PathNode* getParent() const {return _parent;};
    };
};

#endif /* defined(__LastSupper__PathFinder__) */
