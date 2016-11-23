//
//  BattleState.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/20.
//
//

#include "Tasks/PlayerControlState/BattleState.h"

#include "MapObjects/Character.h"
#include "MapObjects/Party.h"
#include "MapObjects/Command/AttackCommand.h"

// コンストラクタ
BattleState::BattleState() { FUNCLOG }

// デストラクタ
BattleState::~BattleState() { FUNCLOG }

// 初期化
bool BattleState::init()
{
    if (!PlayerControlState::init()) return false;
    
    return true;
}

// 決定キーが押された時
void BattleState::onEnterKeyPressed(Party* party)
{
    Character* mainCharacter { party->getMainCharacter() };
    if (mainCharacter->isInAttackMotion()) return;
    
    AttackCommand* command { AttackCommand::create() };
    command->setName(Character::AnimationName::getAttack("attack", mainCharacter->getDirection()));
    
    mainCharacter->pushCommand(command);
}
