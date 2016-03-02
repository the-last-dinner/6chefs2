//
//  BothFollow.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#include "Tasks/FollowType/BothFollow.h"

// コンストラクタ
BothFollow::BothFollow() {FUNCLOG};

// デストラクタ
BothFollow::~BothFollow() {FUNCLOG};

// 初期化
bool BothFollow::init()
{
    if(!FollowType::init()) return false;
    
    return true;
}

// マップを配置すべき座標を取得
Point BothFollow::getPosition(const Size& mapSize, const Point& targetPosition)
{
    return Point(-targetPosition + Director::getInstance()->getWinSize() / 2);
}