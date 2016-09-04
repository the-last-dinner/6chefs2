//
//  Normal.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/03.
//
//

#include "MapObjects/TerrainState/Normal.h"

#include "CocosStudio/CSNode.h"

#include "MapObjects/Character.h"

USING_NS_MO_T_STATE

// コンストラクタ
Normal::Normal() { FUNCLOG }

// デストラクタ
Normal::~Normal() { FUNCLOG }

// 初期化
bool Normal::init()
{
    _isRunnable = true;
    _consumeStaminaWalking = false;
    _staminaConsumptionRatio = 1.f;
    
    return true;
}

void Normal::move(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio)
{
    target->move(directions, [target, onMoved, ratio]
                 {
                     if(onMoved) onMoved();
                     if(!target->isMoving()) target->setDirection(target->getDirection());
                 }, ratio);
}

void Normal::stamp(Character* target, const Direction& direction, const float ratio)
{
    target->playAnimationIfNotPlaying(Character::AnimationName::getWalk(direction), ratio);
}
