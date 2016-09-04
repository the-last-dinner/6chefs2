//
//  Slip.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/03.
//
//

#include "MapObjects/TerrainState/Slip.h"

#include "CocosStudio/CSNode.h"

#include "MapObjects/Character.h"

USING_NS_MO_T_STATE

// コンストラクタ
Slip::Slip() { FUNCLOG };

// デストラクタ
Slip::~Slip() { FUNCLOG };

// 初期化
bool Slip::init()
{
    _isRunnable = true;
    _consumeStaminaWalking = false;
    _staminaConsumptionRatio = 1.f;
    
    return true;
}

void Slip::move(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio)
{
    target->move(directions, [target, directions, onMoved, ratio]
    {
        if(!target->moveBy(directions, onMoved, ratio) && onMoved)  onMoved();
    }, ratio);
}

void Slip::stamp(Character* target, const Direction& direction, const float ratio)
{
    target->playAnimationIfNotPlaying(Character::AnimationName::getTurn(direction), ratio);
}
