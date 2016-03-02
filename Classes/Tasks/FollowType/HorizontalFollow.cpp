//
//  HorizontalFollow.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#include "Tasks/FollowType/HorizontalFollow.h"

// コンストラクタ
HorizontalFollow::HorizontalFollow() {FUNCLOG};

// デストラクタ
HorizontalFollow::~HorizontalFollow() {FUNCLOG};

// 初期化
bool HorizontalFollow::init()
{
    if(!FollowType::init()) return false;
    
    return true;
}

// マップを配置すべき座標を取得
Point HorizontalFollow::getPosition(const Size& mapSize, const Point& targetPosition)
{
    return Point(Director::getInstance()->getWinSize().width / 2 - targetPosition.x, (Director::getInstance()->getWinSize().height - mapSize.height) / 2);
}
