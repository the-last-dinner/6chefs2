//
//  MapUtils.cpp
//  LastSupper
//
//  Created by Kohei on 2015/07/04.
//
//

#include "MapUtils.h"

Point MapUtils::convertToCCPoint(const Size& mapSize, const Point& gridPoint, const Size& objectSize)
{
	return Point(gridPoint.x * GRID + objectSize.width / 2, mapSize.height - gridPoint.y * GRID + (objectSize.height - GRID * 2) / 2);
}

Point MapUtils::convertToMapPoint(const Size& mapSize, const Point& ccPoint)
{
	return Point(ccPoint.x, (mapSize.height - ccPoint.y - GRID));
}

// 画面上の座標を取得
Point MapUtils::convertToDispPosition(const Point& mapPosition, const Point& objectPosition)
{
    return mapPosition + objectPosition;
}


Point MapUtils::getGridVector(const Direction& direction)
{
    map<Direction, Point> directionToVec
    {
        {Direction::FRONT, Point(0, -GRID)},
        {Direction::RIGHT, Point(GRID, 0)},
        {Direction::LEFT, Point(-GRID, 0)},
        {Direction::BACK, Point(0, GRID)},
    };
    
    if(!directionToVec.count(direction)) return Point::ZERO;
    
    return directionToVec[direction];
}

// 長さから、マス数を取得
float MapUtils::getGridNum(float length)
{
    return abs(length) / GRID;
}

// 方向キーを向きへ変換
Direction MapUtils::keyToDirection(const Key& key)
{
    // 変換できないキーの場合はSIZEを返す
    if(static_cast<int>(Direction::SIZE) <= static_cast<int>(key)) return Direction::SIZE;
    
    return static_cast<Direction>(key);
}

// 方向キーベクタを向きへ変換しスタックに詰めて返す
vector<Direction> MapUtils::keyToDirection(const vector<Key>& keys)
{
    vector<Direction> directions {};
    if(keys.empty()) return directions;
    for(Key key : keys)
    {
        Direction direction {MapUtils::keyToDirection(key)};
        if(direction != Direction::SIZE) directions.push_back(direction);
    }
    return directions;
}

// 文字列の方向から、enumへ変換
Direction MapUtils::toEnumDirection(const string& sDirection)
{
    map<string, Direction> stringToDirection
    {
        {string("RIGHT"), Direction::RIGHT},
        {string("LEFT"), Direction::LEFT},
        {string("UP"), Direction::BACK},
        {string("DOWN"), Direction::FRONT},
    };
    
    if(stringToDirection.count(sDirection) == 0) return Direction::SIZE;
    
    return stringToDirection.at(sDirection);
}

// マップ上ベクトルから方向へ変換
Direction MapUtils::vecToMapDirection(const Vec2& vec)
{
    if(vec.x < 0) return Direction::LEFT;
    if(vec.x > 0) return Direction::RIGHT;
    if(vec.y < 0) return Direction::BACK;
    if(vec.y > 0) return Direction::FRONT;
    
    return Direction::SIZE;
}

// マップ上ベクトルから方向へ変換
vector<Direction> MapUtils::vectoMapDirections(const Vec2& vec)
{
    vector<Direction> directions {};
    
    if(vec.x > 0)
    {
        directions.push_back(Direction::RIGHT);
    }
    else if(vec.x < 0)
    {
        directions.push_back(Direction::LEFT);
    }
    
    if(vec.y > 0)
    {
        directions.push_back(Direction::FRONT);
    }
    else if(vec.y < 0)
    {
        directions.push_back(Direction::BACK);
    }
    
    return directions;
}

// ベクトルから方向へ変換
vector<Direction> MapUtils::vecToDirections(const Vec2& vec)
{
    vector<Direction> directions {};
    
    if(vec.x > 0)
    {
        directions.push_back(Direction::RIGHT);
    }
    else if(vec.x < 0)
    {
        directions.push_back(Direction::LEFT);
    }
    
    if(vec.y > 0)
    {
        directions.push_back(Direction::BACK);
    }
    else if(vec.y < 0)
    {
        directions.push_back(Direction::FRONT);
    }
    
    return directions;
}

// 方向から単位ベクトル取得
Vec2 MapUtils::getUnitVector(const Direction& direction)
{
    map<Direction, Point> directionToVec
    {
        {Direction::FRONT, Point(0, -1)},
        {Direction::RIGHT, Point(1, 0)},
        {Direction::LEFT, Point(-1, 0)},
        {Direction::BACK, Point(0, 1)},
    };
    
    if(!directionToVec.count(direction)) return Point::ZERO;
    
    return directionToVec[direction];
}

// マップRect用衝突判定メソッド（Y軸が反転しているため必要）
bool MapUtils::intersectsGridRect(const Rect& rect1, const Rect& rect2)
{
    return !(rect1.getMaxX() - 1 < rect2.getMinX() ||
             rect1.getMinX() > rect2.getMaxX() - 1||
             rect1.origin.y - rect1.size.height + 1 > rect2.getMinY() ||
             rect1.getMinY() < rect2.origin.y - rect2.size.height + 1);
}

// マスRect用内包判定メソッド
bool MapUtils::includesGridRect(const cocos2d::Rect& parent, const Rect& child)
{
    return !(parent.getMinX() > child.getMinX() ||
             parent.getMaxX() < child.getMaxX() ||
             parent.getMinY() < child.getMinY() ||
             parent.getMinY() - parent.size.height > child.getMinY() - child.size.height);
}

// 反対方向を取得
Direction MapUtils::oppositeDirection(const Direction& direction)
{
    map<Direction, Direction> directionMap
    {
        {Direction::FRONT, Direction::BACK},
        {Direction::BACK, Direction::FRONT},
        {Direction::RIGHT, Direction::LEFT},
        {Direction::LEFT, Direction::RIGHT},
    };
    
    if(directionMap.count(direction) == 0) return Direction::SIZE;
    
    return directionMap[direction];
}

// 方向ベクタからマップ上ベクトルに変換
Vec2 MapUtils::directionsToMapVector(const vector<Direction>& directions)
{
    Vec2 vec {Vec2::ZERO};
    
    if(directions.empty()) return vec;
    
    map<Direction, Vec2> dirToVec2
    {
        {Direction::BACK, Vec2(0, -1)},
        {Direction::FRONT, Vec2(0, 1)},
        {Direction::LEFT, Vec2(-1, 0)},
        {Direction::RIGHT, Vec2(1, 0)},
    };
    
    for(Direction direction : directions)
    {
        if(dirToVec2.count(direction) == 0) continue;
        vec += dirToVec2[direction];
    }
    
    return vec;
}