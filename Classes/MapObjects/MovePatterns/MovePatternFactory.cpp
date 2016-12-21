//
//  MovePatternFactory.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#include "MapObjects/MovePatterns/MovePatternFactory.h"

#include "MapObjects/MovePatterns/Chaser.h"
#include "MapObjects/MovePatterns/CheapChaser.h"
#include "MapObjects/MovePatterns/MobRandom.h"
#include "MapObjects/MovePatterns/RandomMove.h"
#include "MapObjects/MovePatterns/Scouter.h"
#include "MapObjects/MovePatterns/BattleMob.h"
#include "MapObjects/MovePatterns/BattleBoss.h"

// コンストラクタ
MovePatternFactory::MovePatternFactory() { FUNCLOG }

// デストラクタ
MovePatternFactory::~MovePatternFactory() { FUNCLOG }

// 初期化
bool MovePatternFactory::init()
{
    return true;
}

MovePattern* MovePatternFactory::createMovePattern(const EnemyMovePattern type, Character* character)
{
    if (type == EnemyMovePattern::SIZE) return nullptr;
    
    map<EnemyMovePattern, function<MovePattern*(Character*)>> typeToFunc
    {
        {EnemyMovePattern::CHASER, Chaser::create},
        {EnemyMovePattern::CHEAP_CHASER, CheapChaser::create},
        {EnemyMovePattern::PERFECT_RANDOM, RandomMove::create},
        {EnemyMovePattern::SCOUTER, Scouter::create},
        {EnemyMovePattern::BATTLE_MOB, BattleMob::create},
        {EnemyMovePattern::BATTLE_BOSS, BattleBoss::create},
    };
    
    if (typeToFunc.count(type) == 0) return nullptr;
    
    return typeToFunc[type](character);
}

MovePattern* MovePatternFactory::createMovePattern(const CharacterMovePattern type, Character* character)
{
    if (type == CharacterMovePattern::SIZE) return nullptr;
    
    map<CharacterMovePattern, function<MovePattern*(Character*)>> typeToFunc
    {
        {CharacterMovePattern::RANDOM, MobRandom::create}
    };
    
    if (typeToFunc.count(type) == 0) return nullptr;
    
    return typeToFunc[type](character);
    
    return nullptr;
}
