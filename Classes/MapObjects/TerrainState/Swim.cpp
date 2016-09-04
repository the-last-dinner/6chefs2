//
//  Swim.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/03.
//
//

#include "MapObjects/TerrainState/Swim.h"

#include "CocosStudio/CSNode.h"

#include "MapObjects/Character.h"

USING_NS_MO_T_STATE

// コンストラクタ
Swim::Swim() { FUNCLOG }

// デストラクタ
Swim::~Swim() { FUNCLOG }

// 初期化
bool Swim::init()
{
    _isRunnable = false;
    _consumeStaminaWalking = true;
    _staminaConsumptionRatio = 2.f;
    
    return true;
}

void Swim::move(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio)
{
    target->move(directions, [target, onMoved, ratio]
    {
        if(onMoved) onMoved();
        target->runAction(Sequence::createWithTwoActions(DelayTime::create(MapObject::DURATION_MOVE_ONE_GRID),
                                                         CallFunc::create([target]
        {
            if(!target->isMoving()) target->setDirection(target->getDirection());
        })));
    }, ratio / 3.f);
}

void Swim::stamp(Character* target, const Direction& direction, const float ratio)
{
    target->playAnimationIfNotPlaying(Character::AnimationName::getSwim(direction), ratio / 3.f);
}
