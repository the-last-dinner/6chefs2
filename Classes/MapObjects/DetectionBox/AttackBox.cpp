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
bool AttackBox::init(MapObject* parent, Node* origin, function<void()> onAttackHitted)
{
    if (!DetectionBox::init(parent, origin)) return false;
    
    _onAttackHitted = onAttackHitted;
    
    return true;
}

// 初期化
bool AttackBox::init(MapObject* parent, const Rect& originRect, function<void()> onAttackHitted)
{
    if (!DetectionBox::init(parent, originRect)) return false;
    
    _onAttackHitted = onAttackHitted;
    
    return true;
}

void AttackBox::onAttackHitted()
{
    if (_onAttackHitted) {
        _onAttackHitted();
    }
}
