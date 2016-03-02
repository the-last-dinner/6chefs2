//
//  MapUtils.h
//  LastSupper
//
//  Created by Kohei on 2015/07/04.
//
//

#ifndef __MAP_UTILS_H__
#define __MAP_UTILS_H__

#include "define.h"

namespace MapUtils
{
    cocos2d::Point convertToMapPoint(const cocos2d::Size& mapSize, const cocos2d::Point& ccPoint);
    cocos2d::Point convertToCCPoint(const cocos2d::Size& mapSize, const cocos2d::Point& gridPoint, const cocos2d::Size& objectSize);
    cocos2d::Point convertToDispPosition(const cocos2d::Point& mapPosition, const cocos2d::Point& objectPosition);
    cocos2d::Point getGridVector(const Direction& direction);
    float   getGridNum(float length);
    Direction keyToDirection(const Key& key);
    vector<Direction> keyToDirection(const vector<Key>& keys);
    Direction toEnumDirection(const string& sDirection);
    Direction vecToMapDirection(const Vec2& vec);
    vector<Direction> vectoMapDirections(const Vec2& vec);
    vector<Direction> vecToDirections(const Vec2& vec);
    Vec2 getUnitVector(const Direction& direction);
    bool intersectsGridRect(const cocos2d::Rect& rect1, const cocos2d::Rect& rect2);
    bool includesGridRect(const Rect& parent, const Rect& child);
    Direction oppositeDirection(const Direction& direction);
    Vec2 directionsToMapVector(const vector<Direction>& directions);
}

#endif // __MAP_UTILS_H__