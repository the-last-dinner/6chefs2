//
//  PathNode.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/16.
//
//

#include "MapObjects/PathFinder/PathNode.h"

// コンストラクタ
PathNode::PathNode() {}

// デストラクタ
PathNode::~PathNode() {}

// 初期化
bool PathNode::init(const Point& gridPoint)
{
    _gridPoint = gridPoint;
    
    return true;
}

void PathNode::setState(const State state)
{
    _state = state;
}

void PathNode::setActualCost(const int cost)
{
    _actualCost = cost;
}

void PathNode::setEstimatedCost(const int cost)
{
    _estimatedCost = cost;
}

void PathNode::setParent(PathNode* node)
{
    _parent = node;
}

Point PathNode::getGridPoint() const
{
    return _gridPoint;
}

PathNode::State PathNode::getState() const
{
    return _state;
}

int PathNode::getActualCost() const
{
    return _actualCost;
}

int PathNode::getEstimatedCost() const
{
    return _estimatedCost;
}

int PathNode::getScore() const
{
    return _actualCost + _estimatedCost;
}

PathNode* PathNode::getParent() const
{
    return _parent;
}
