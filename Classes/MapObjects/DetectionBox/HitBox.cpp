//
//  HitBox.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/19.
//
//

#include "MapObjects/DetectionBox/HitBox.h"

// コンストラクタ
HitBox::HitBox() { FUNCLOG }

// デストラクタ
HitBox::~HitBox() { FUNCLOG }

// 初期化
bool HitBox::init(MapObject* parent, Node* origin)
{
    if(!DetectionBox::init(parent, origin)) return false;
    
    return true;
}

// 初期化
bool HitBox::init(MapObject* parent, const Rect& originRect)
{
    if(!DetectionBox::init(parent, originRect)) return false;
    
    return true;
}
