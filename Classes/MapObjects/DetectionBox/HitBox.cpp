//
//  HitBox.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/19.
//
//

#include "MapObjects/DetectionBox/HitBox.h"

#include "MapObjects/MapObject.h"

// コンストラクタ
HitBox::HitBox() { FUNCLOG }

// デストラクタ
HitBox::~HitBox() { FUNCLOG }

// 初期化
bool HitBox::init(MapObject* parent, Node* origin, function<void(int)> onHitted)
{
    if (!DetectionBox::init(parent, origin)) return false;
    
    _onHitted = onHitted;
    
    return true;
}

// 初期化
bool HitBox::init(MapObject* parent, const Rect& originRect, function<void(int)> onHitted)
{
    if (!DetectionBox::init(parent, originRect)) return false;
    
    _onHitted = onHitted;
    
    return true;
}

void HitBox::onHitted(int damage)
{
    if (_onHitted) {
        _onHitted(damage);
    }
}

#pragma mark -
#pragma mark Interface

bool HitBox::intersects(const DetectionBox* other) const
{
    if (!other->getMOParent()->canAttack(this->getMOParent())) return false;
    return DetectionBox::intersects(other);
}

#pragma mark -
#pragma mark Debug

Color4F HitBox::getDebugMaskColor() const
{
    return Color4F::BLUE;
}
