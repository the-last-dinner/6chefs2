//
//  PathNode.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/16.
//
//

#ifndef PathNode_h
#define PathNode_h

#include "define.h"

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
    Point _gridPoint { Point::ZERO };
    State _state { State::NONE };
    int _actualCost { 0 };
    int _estimatedCost { 0 };
    PathNode* _parent { nullptr };
    
private:
    PathNode();
    virtual ~PathNode();
    bool init(const Point& gridPoint);
public:
    void setState(const State state);
    void setActualCost(const int cost);
    void setEstimatedCost(const int cost);
    void setParent(PathNode* node);
    
    Point getGridPoint() const;
    State getState() const;
    int getActualCost() const;
    int getEstimatedCost() const;
    int getScore() const;
    PathNode* getParent() const;
};

#endif /* PathNode_h */
