//
//  AttackDetector.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/19.
//
//

#include "MapObjects/DetectionBox/AttackDetector.h"

// コンストラクタ
AttackDetector::AttackDetector() { FUNCLOG }

// デストラクタ
AttackDetector::~AttackDetector()
{
    FUNCLOG
    
    _attackBoxes.clear();
    _hitBoxes.clear();
}

// 初期化
bool AttackDetector::init()
{
    if(!Node::init()) return false;
    
    return true;
}

void AttackDetector::addAttackBox(AttackBox* attackBox)
{
    if (!attackBox) return;
    
    _attackBoxes.pushBack(attackBox);
}

void AttackDetector::removeAttackBox(AttackBox* attackBox)
{
    if (!attackBox) return;
    
    _attackBoxes.eraseObject(attackBox);
}

void AttackDetector::addHitBox(HitBox* hitBox)
{
    if (!hitBox) return;
    
    _hitBoxes.pushBack(hitBox);
}

void AttackDetector::removeHitBox(HitBox* hitBox)
{
    if (!hitBox) return;
    
    _hitBoxes.eraseObject(hitBox);
}

HitBox* AttackDetector::getIntersectHitBox(AttackBox* attackBox)
{
    for (HitBox* hitBox : _hitBoxes) {
        if(hitBox->intersects(attackBox)) return hitBox;
    }
    
    return nullptr;
}

void AttackDetector::update(float delta)
{
    for (AttackBox* attackBox : _attackBoxes) {
        attackBox->update(delta);
    }
    
    for (HitBox* hitBox : _hitBoxes) {
        hitBox->update(delta);
    }
    
    for(AttackBox* attackBox : _attackBoxes) {
        HitBox* hitBox { this->getIntersectHitBox(attackBox) };
        
        if (!hitBox) continue;
        
        attackBox->onAttackHitted();
        hitBox->onHitted();
    }
}
