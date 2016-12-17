//
//  CollisionBox.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/11.
//
//

#include "MapObjects/DetectionBox/CollisionBox.h"

// コンストラクタ
CollisionBox::CollisionBox() {}

// デストラクタ
CollisionBox::~CollisionBox() {}

// 初期化
bool CollisionBox::init(MapObject* parent, Node* origin)
{
    if (!DetectionBox::init(parent, origin)) return false;
    
    return true;
}

bool CollisionBox::init(MapObject* parent, const Rect& originRect)
{
    if (!DetectionBox::init(parent, originRect)) return false;
    
    return true;
}

#pragma mark -
#pragma mark Debug
Color4F CollisionBox::getDebugMaskColor() const
{
    return Color4F::GREEN;
}
