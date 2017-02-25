//
//  Direction.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/10.
//
//

#include "define.h"

#pragma mark Direction

// 定数
const Direction Direction::NONE { Direction() };
const Direction Direction::UP { DirectionUp() };
const Direction Direction::DOWN { DirectionDown() };
const Direction Direction::RIGHT { DirectionRight() };
const Direction Direction::LEFT { DirectionLeft() };

const map<Key, Direction> Direction::KEY_TO_DIRECTION
{
    { Key::UP, Direction::UP },
    { Key::DOWN, Direction::DOWN },
    { Key::RIGHT, Direction::RIGHT },
    { Key::LEFT, Direction::LEFT },
};

const map<string, Direction> Direction::STR_TO_DIRECTION
{
    { "UP", Direction::UP },
    { "DOWN", Direction::DOWN },
    { "RIGHT", Direction::RIGHT },
    { "LEFT", Direction::LEFT },
};

const map<int, Direction> Direction::INT_TO_DIRECTION
{
    { 3, Direction::UP },
    { 0, Direction::DOWN },
    { 1, Direction::RIGHT },
    { 2, Direction::LEFT },
};

const map<int, Direction> Direction::INT_TO_OPPOSITE_DIRECTION
{
    { 3, Direction::DOWN },
    { 0, Direction::UP },
    { 1, Direction::LEFT },
    { 2, Direction::RIGHT },
};

// クラスメソッド

Direction Direction::convertKey(const Key& key)
{
    if(KEY_TO_DIRECTION.count(key) == 0) return Direction();
    
    return KEY_TO_DIRECTION.at(key);
}

vector<Direction> Direction::convertKeys(const vector<Key>& keys)
{
    vector<Direction> directions {};
    
    for(Key key : keys)
    {
        directions.push_back(Direction::convertKey(key));
    }
    
    return directions;
}

Direction Direction::convertString(const string& directionStr)
{
    if(STR_TO_DIRECTION.count(directionStr) == 0) return Direction();
    
    return STR_TO_DIRECTION.at(directionStr);
}

Direction Direction::convertInt(const int directionInt)
{
    if(INT_TO_DIRECTION.count(directionInt) == 0) return Direction();
    
    return INT_TO_DIRECTION.at(directionInt);
}

Direction Direction::convertVec2(const Vec2& vec2)
{
    if(vec2.x > 0) return Direction::RIGHT;
    if(vec2.x < 0) return Direction::LEFT;
    
    if(vec2.y > 0) return Direction::UP;
    if(vec2.y < 0) return Direction::DOWN;
    
    return Direction::NONE;
}

vector<Direction> Direction::convertGridVec2(const Vec2& vec2)
{
    vector<Direction> directions {};
    
    if(vec2.x > 0)
    {
        directions.push_back(Direction::RIGHT);
    }
    else if(vec2.x < 0)
    {
        directions.push_back(Direction::LEFT);
    }
    
    if(vec2.y > 0)
    {
        directions.push_back(Direction::DOWN);
    }
    else if(vec2.y < 0)
    {
        directions.push_back(Direction::UP);
    }
    
    return directions;
}

Vec2 Direction::getVec2(const vector<Direction>& directions)
{
    Vec2 vec2 { Vec2::ZERO };
    
    for(Direction direction : directions)
    {
        vec2 += direction.getVec2();
    }
    
    return vec2;
}

Vec2 Direction::getGridVec2(const vector<Direction>& directions)
{
    Vec2 vec2 { Vec2::ZERO };
    
    for(Direction direction : directions)
    {
        vec2 += direction.getGridVec2();
    }
    
    return vec2;
}

vector<Direction> Direction::getAll()
{
    return { UP, DOWN, RIGHT, LEFT };
}

// インスタンスメソッド

Direction::Direction() {}


Direction::~Direction() {}

bool Direction::isNull() const
{
    return _isNull;
}

int Direction::getInt() const
{
    return _int;
}

float Direction::getAngle() const
{
    return _angle;
}

string Direction::getUpcaseString() const
{
    return _upcaseString;
}

string Direction::getDowncaseString() const
{
    return _downcaseString;
}

Vec2 Direction::getUnitVec2() const
{
    return _unitVec2;
}

Vec2 Direction::getVec2() const
{
    return this->getUnitVec2() * GRID;
}

Vec2 Direction::getGridVec2() const
{
    return _gridVec2;
}

Direction Direction::getOppositeDirection() const
{
    return INT_TO_OPPOSITE_DIRECTION.at(this->getInt());
}

bool Direction::isVertical() const
{
    return _vertical;
}

int Direction::getDegree() const
{
    return _degree;
}

Direction Direction::convertToWorldDirection(const Direction& direction) const
{
    int degree { this->getDegree() };
    int si { static_cast<int>(sin(degree)) };
    int co { static_cast<int>(cos(degree)) };
    Vec2 uVec2 { Vec2(co, -si) };
    Vec2 lVec2 { Vec2(si, co) };
    
    Vec2 vec2 { direction.getUnitVec2() };
    
    Vec2 cVec2 { Vec2(vec2.dot(uVec2), vec2.dot(lVec2)) };
    
    return Direction::convertVec2(cVec2);
}

#pragma mark -
#pragma mark DirectionUp

DirectionUp::DirectionUp()
{
    _isNull = false;
    _int = 3;
    _angle = 0;
    _upcaseString = "UP";
    _downcaseString = "up";
    _unitVec2 = Vec2(0, 1);
    _gridVec2 = Vec2(0, -1);
    _degree = 0;
    _vertical = true;
}

#pragma mark -
#pragma mark DirectionDown

DirectionDown::DirectionDown()
{
    _isNull = false;
    _int = 0;
    _angle = 180;
    _upcaseString = "DOWN";
    _downcaseString = "down";
    _unitVec2 = Vec2(0, -1);
    _gridVec2 = Vec2(0, 1);
    _degree = 180;
    _vertical = true;
}

#pragma mark -
#pragma mark DirectionRight

DirectionRight::DirectionRight()
{
    _isNull = false;
    _int = 1;
    _angle = 90;
    _upcaseString = "RIGHT";
    _downcaseString = "right";
    _unitVec2 = Vec2(1, 0);
    _gridVec2 = Vec2(1, 0);
    _degree = 90;
}

#pragma mark -
#pragma mark DirectionLeft

DirectionLeft::DirectionLeft()
{
    _isNull = false;
    _int = 2;
    _angle = 270;
    _upcaseString = "LEFT";
    _downcaseString = "left";
    _unitVec2 = Vec2(-1, 0);
    _gridVec2 = Vec2(-1, 0);
    _degree = 270;
}
