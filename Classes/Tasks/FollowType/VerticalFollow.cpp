//
//  VerticalFollow.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#include "Tasks/FollowType/VerticalFollow.h"

// コンストラクタ
VerticalFollow::VerticalFollow() {FUNCLOG};

// デストラクタ
VerticalFollow::~VerticalFollow() {FUNCLOG};

// 初期化
bool VerticalFollow::init()
{
    if(!FollowType::init()) return false;
    
    return true;
}

// マップを配置すべき座標を取得
Point VerticalFollow::getPosition(const Size& mapSize, const Point& targetPosition)
{
    return Point((Director::getInstance()->getWinSize().width - mapSize.width) / 2, Director::getInstance()->getWinSize().height / 2 - targetPosition.y);
}