//
//  Sight.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#include "MapObjects/Status/Sight.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/DetectionBox/CollisionDetector.h"

// 定数
const float Sight::DEFAULT_VIEW_ANGLE { 90.f };
const int Sight::DEFAULT_GRID_LIMIT_DISTANCE { 10 };

// コンストラクタ
Sight::Sight() { FUNCLOG }

// デストラクタ
Sight::~Sight() { FUNCLOG }

// 初期化
bool Sight::init(Character* chara)
{
    _chara = chara;
    
    return true;
}

// 対象が視界内にいるか
bool Sight::isIn(const MapObject* target, const MapObjectList* list) const
{
    // キャラクタの向いている向きのベクトルを取得
    Vec2 v1 { _chara->getDirection().getVec2() };
    
    // キャラクタから対象の間にできるベクトル
    Point p1 { _chara->getPosition() };
    Point p2 { target->getPosition() };
    Vec2 v2 { p2 - p1 };
    
    // 視界限界距離より遠くにいれば視界外
    if (v2.getLength() > _limitDistance * GRID) return false;
    
    // 二本のベクトルの間にできる角度を取得
    float degree { fabs(MapUtils::radianToDegree(v1.getAngle(v2))) };
    
    // 視野角の半分に収まっていなければ視界外
    if (degree > _angle / 2) return false;
    
    // 対象と本人の間に当たり判定があれば視界外
    if (list->getCollisionDetector()->existsCollisionBetween(_chara, target)) return false;
    
    return true;
}
