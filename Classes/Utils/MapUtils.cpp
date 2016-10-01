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

// 長さから、マス数を取得
float MapUtils::getGridNum(float length)
{
    return abs(length) / GRID;
}

// マップRect用衝突判定メソッド（Y軸が反転しているため必要）
bool MapUtils::intersectsGridRect(const cocos2d::Rect& rect1, const cocos2d::Rect& rect2)
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

// 線分がRectに交差しているかどうか
bool MapUtils::isSegmentIntersectWithRect(const Point& p1, const Point& p2, const cocos2d::Rect& rect)
{
    // 上下左右の線分とそれぞれ交差しているか
    if(Vec2::isSegmentIntersect(p1, p2, Point(rect.getMinX(), rect.getMaxY()), Point(rect.getMaxX(), rect.getMaxY()))) return true;
    if(Vec2::isSegmentIntersect(p1, p2, Point(rect.getMinX(), rect.getMinY()), Point(rect.getMaxX(), rect.getMinY()))) return true;
    if(Vec2::isSegmentIntersect(p1, p2, Point(rect.getMaxX(), rect.getMinY()), Point(rect.getMaxX(), rect.getMaxY()))) return true;
    if(Vec2::isSegmentIntersect(p1, p2, Point(rect.getMinX(), rect.getMinY()), Point(rect.getMinX(), rect.getMaxY()))) return true;
    
    return false;
}

// 弧度法から度数法に変換
float MapUtils::radianToDegree(float radian)
{
    return (radian * 180.f) / 3.14159265359f;
}

Vec2 MapUtils::gridVecToVec2(const cocos2d::Vec2& gridVec)
{
    Vec2 vec2 { gridVec.x * GRID, -gridVec.y * GRID};
    
    return vec2;
}
