//
//  AttackBox.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/19.
//
//

#include "MapObjects/DetectionBox/AttackBox.h"

// コンストラクタ
AttackBox::AttackBox() { FUNCLOG }

// デストラクタ
AttackBox::~AttackBox() { FUNCLOG }

// 初期化
bool AttackBox::init(MapObject* parent, Node* origin)
{
    if(!DetectionBox::init(parent, origin)) return false;
    
    return true;
}

// 初期化
bool AttackBox::init(MapObject* parent, const Rect& originRect)
{
    if(!DetectionBox::init(parent, originRect)) return false;
    
    return true;
}
