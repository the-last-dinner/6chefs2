//
//  Ladder.cpp
//  6chefs2
//
//  Created by S.inami on 2016/12/16.
//
//

#include "MapObjects/TerrainState/Ladder.h"

#include "CocosStudio/CSNode.h"

#include "MapObjects/Character.h"

USING_NS_MO_T_STATE

// コンストラクタ
Ladder::Ladder() { FUNCLOG };

// デストラクタ
Ladder::~Ladder() { FUNCLOG };

// 初期化
bool Ladder::init()
{
    _isRunnable = true;
    _consumeStaminaWalking = false;
    _staminaConsumptionRatio = 1.f;
    // 上以外は方向転換させない
    map<int, bool> directionIntToturnable = {
        { Direction::UP.getInt(),   true  },
        { Direction::DOWN.getInt(), false },
        { Direction::RIGHT.getInt(),false },
        { Direction::LEFT.getInt(), false },
    };
    _directionIntToturnable = directionIntToturnable;
    
    return true;
}

void Ladder::move(MapObject* target, const vector<Direction>& directions, function<void()> cb, float speed)
{
    target->move(directions, cb, speed);
}

void Ladder::stamp(Character* target, const Direction& direction, float speed)
{
    target->playAnimationIfNotPlaying(Character::AnimationName::getWalk(Direction::UP), speed);
}
