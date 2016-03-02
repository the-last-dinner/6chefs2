//
//  NeverFollow.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#include "Tasks/FollowType/NeverFollow.h"

// コンストラクタ
NeverFollow::NeverFollow() {FUNCLOG};

// デストラクタ
NeverFollow::~NeverFollow() {FUNCLOG};

// 初期化
bool NeverFollow::init()
{
    if(!FollowType::init()) return false;
    
    return true;
}

// マップを配置すべき座標を取得
Point NeverFollow::getPosition(const Size& mapSize, const Point& targetPosition)
{
    return Point((Director::getInstance()->getWinSize() - mapSize) / 2);
}