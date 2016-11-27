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

void Swim::move(MapObject* target, const vector<Direction>& directions, function<void()> cb, float speed)
{
    target->move(directions, cb, speed / 3.f);
}

void Swim::stamp(Character* target, const Direction& direction, float speed)
{
    target->playAnimationIfNotPlaying(Character::AnimationName::getSwim(direction), speed / 3.f);
}
