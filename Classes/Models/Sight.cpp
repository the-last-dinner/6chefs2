//
//  Sight.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#include "Models/Sight.h"

#include "MapObjects/MapObjectList.h"

// 定数
const float Sight::DEFAULT_VIEW_ANGLE { 90.f };
const int Sight::DEFAULT_GRID_LIMIT_DISTANCE { 10 };

// コンストラクタ
Sight::Sight() {FUNCLOG};

// デストラクタ
Sight::~Sight() {FUNCLOG};

// 初期化
bool Sight::init(Character* chara)
{
    this->chara = chara;
    
    return true;
}

// 弧度法から度数法に変換
float Sight::toDegree(const float radian) const
{
    return (radian * 180.f) / 3.14159265359f;
}

// 線分がRectの左辺と交差しているか
bool Sight::isSegmentIntersectWithLeft(const Point& p1, const Point& p2, const Rect& rect) const
{
    return Vec2::isSegmentIntersect(p1, p2, Point(rect.getMinX(), rect.getMinY()), Point(rect.getMinX(), rect.getMaxY()));
}

// 線分がRectの右辺と交差しているか
bool Sight::isSegmentIntersectWithRight(const Point& p1,const Point& p2, const Rect& rect) const
{
    return Vec2::isSegmentIntersect(p1, p2, Point(rect.getMaxX(), rect.getMinY()), Point(rect.getMaxX(), rect.getMaxY()));
}

// 線分がRectの上辺と交差しているか
bool Sight::isSegmentIntersectWithUpper(const Point& p1, const Point& p2, const Rect& rect) const
{
    return Vec2::isSegmentIntersect(p1, p2, Point(rect.getMinX(), rect.getMaxY()), Point(rect.getMaxX(), rect.getMaxY()));
}

// 線分がRectの下辺と交差しているか
bool Sight::isSegmentIntersectWithBottom(const Point& p1, const Point& p2, const Rect& rect) const
{
    return Vec2::isSegmentIntersect(p1, p2, Point(rect.getMinX(), rect.getMinY()), Point(rect.getMaxX(), rect.getMinY()));
}

// 対象が視界内にいるか
bool Sight::isIn(MapObject* target, MapObjectList* list) const
{
    // キャラクタの向いている向きのベクトルを取得
    Vec2 v1 { MapUtils::getGridVector(this->chara->getDirection()) };
    
    // 対象の当たり判定Rect
    Rect rect {target->getCollisionRect()};
    
    // キャラクタから対象の間にできるベクトル
    Point p1 {this->chara->getPosition()};
    Point p2 {Point(rect.getMidX(), rect.getMidY())};
    Vec2 v2 { p2 - p1 };
    
    // 視界限界距離より遠くにいれば視界外
    if(v2.getLength() > this->limitDistance * GRID) return false;
    
    // 二本のベクトルの間にできる角度を取得
    float degree { fabs(this->toDegree(v1.getAngle(v2))) };
    
    // 視野角の半分に収まっていなければ視界外
    if(degree > this->angle / 2) return false;
    
    // 対象と本人以外の当たり判定用Rectを取得
    vector<Rect> collisionRects {list->getCollisionRects({target, this->chara})};
    
    // Rectを４辺に分解し、それぞれの辺ベクトルとv2が交差しているかチェックする
    for(Rect rect : collisionRects)
    {
        if(this->isSegmentIntersectWithLeft(p1, p2, rect)) return false;
        if(this->isSegmentIntersectWithRight(p1, p2, rect)) return false;
        if(this->isSegmentIntersectWithUpper(p1, p2, rect)) return false;
        if(this->isSegmentIntersectWithBottom(p1, p2, rect)) return false;
    }
    
    return true;
}