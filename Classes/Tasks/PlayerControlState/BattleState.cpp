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

#include "MapObjects/Status/Stamina.h"

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

// 移動
void BattleState::move(Party* party, const vector<Direction>& directions, bool isDashKeyPressed)
{
    Character* mainCharacter { party->getMainCharacter() };
    bool step { mainCharacter->isRunnable() ? isDashKeyPressed : false };
    
    // 入力から、使う方向の個数を決める
    int directionCount { (directions.size() == 2 && !Direction::getVec2({directions.front(), directions.back()}).isZero()) ? 2 : 1 };
    
    vector<Direction> moveDirections {};
    for (int i { 0 }; i < directions.size(); i++) {
        if (directions.size() - directionCount > i) continue;
        moveDirections.push_back(directions.at(i));
    }
    
    Stamina* stamina { DungeonSceneManager::getInstance()->getStamina() };
    party->move(moveDirections, 1.f, [this, party, stamina, step](bool moved) {
        if (!moved) return;
        
        stamina->setDecreasing(false);
        _task->onPartyMovedOneGrid(party, false);
    });
}

#pragma mark -
#pragma mark Callback

// 攻撃コマンド終了時
void BattleState::onAttackCommandFinished(Party* party)
{
    _task->move(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
}
