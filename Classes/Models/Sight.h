//
//  Sight.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#ifndef Sight_h
#define Sight_h

#include "define.h"

class Character;
class MapObject;
class MapObjectList;

class Sight : public Ref
{
// 定数
    static const float DEFAULT_VIEW_ANGLE;
    static const int DEFAULT_GRID_LIMIT_DISTANCE;

// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Sight, Character*)
    
// インスタンス変数
private:
    Character* chara { nullptr };
    float angle { DEFAULT_VIEW_ANGLE };
    int limitDistance { DEFAULT_GRID_LIMIT_DISTANCE };
    
// インスタンスメソッド
private:
    Sight();
    ~Sight();
    bool init(Character* chara);
    float toDegree(const float radian) const;
    bool isSegmentIntersectWithLeft(const Point& p1, const Point& p2, const Rect& rect) const;
    bool isSegmentIntersectWithRight(const Point& p1, const Point& p2, const Rect& rect) const;
    bool isSegmentIntersectWithUpper(const Point& p1, const Point& p2, const Rect& rect) const;
    bool isSegmentIntersectWithBottom(const Point& p1, const Point& p2, const Rect& rect) const;
public:
    bool isIn(MapObject* target, MapObjectList* list) const;
};

#endif /* Sight_h */
