//
//  BattleMob.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/11.
//
//

#include "MapObjects/MovePatterns/BattleMob.h"

#include "MapObjects/MovePatterns/Chaser.h"

// コンストラクタ
BattleMob::BattleMob() { FUNCLOG }

// デストラクタ
BattleMob::~BattleMob()
{
    FUNCLOG
    
    CC_SAFE_RELEASE_NULL(_subPattern);
}

// 初期化
bool BattleMob::init(Character* character)
{
    if (!MovePattern::init(character)) return false;
    
    return true;
}


#pragma mark -
#pragma mark Interface
void BattleMob::start()
{
    MovePattern::start();
}

void BattleMob::pause()
{
    MovePattern::pause();
}

void BattleMob::resume()
{
    MovePattern::resume();
}
