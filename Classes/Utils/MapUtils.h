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
    float getGridNum(float length);
    bool intersectsGridRect(const cocos2d::Rect& rect1, const cocos2d::Rect& rect2);
    bool includesGridRect(const Rect& parent, const Rect& child);
    bool isSegmentIntersectWithRect(const Point& p1, const Point& p2, const Rect& rect);
    float radianToDegree(float radian);
    Vec2 gridVecToVec2(const Vec2& gridVec);
}

#endif // __MAP_UTILS_H__
