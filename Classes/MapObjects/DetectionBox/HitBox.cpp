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
bool HitBox::init(MapObject* parent, Node* origin, function<void()> onHitted)
{
    if(!DetectionBox::init(parent, origin)) return false;
    
    _onHitted = onHitted;
    
    return true;
}

// 初期化
bool HitBox::init(MapObject* parent, const Rect& originRect, function<void()> onHitted)
{
    if(!DetectionBox::init(parent, originRect)) return false;
    
    _onHitted = onHitted;
    
    return true;
}

void HitBox::onHitted()
{
    if(_onHitted)
    {
        _onHitted();
    }
}
