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

#include "Managers/DungeonSceneManager.h"

#include "Tasks/PlayerControlTask.h"

// コンストラクタ
BattleState::BattleState() { FUNCLOG }

// デストラクタ
BattleState::~BattleState() { FUNCLOG }

// 初期化
bool BattleState::init(PlayerControlTask* task)
{
    if (!PlayerControlState::init(task)) return false;
    return true;
}

#pragma mark -
#pragma mark Interface

// 決定キーが押された時
void BattleState::onEnterKeyPressed(Party* party)
{
    Character* mainCharacter { party->getMainCharacter() };
    if (mainCharacter->isInAttackMotion()) return;
    
    AttackCommand* command { AttackCommand::create() };
    command->setName(Character::AnimationName::getAttack("attack", mainCharacter->getDirection()));
    command->setCallback([this, party](Character* c) {
        this->onAttackCommandFinished(party);
    });
    
    mainCharacter->pushCommand(command);
}

#pragma mark -
#pragma mark Callback

// 攻撃コマンド終了時
void BattleState::onAttackCommandFinished(Party* party)
{
    _task->walk(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
}
