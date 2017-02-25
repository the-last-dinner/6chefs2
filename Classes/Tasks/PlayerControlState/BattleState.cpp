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
#include "Managers/BattleManager.h"

#include "MapObjects/Status/Stamina.h"

#include "Tasks/PlayerControlTask.h"

#include "MapObjects/Command/StepCommand.h"

const float BattleState::STEP_COOLDOWN_DURATION { 0.6f };

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

// 振り向き
void BattleState::turn(Party* party, const Direction& direction, bool isDashKeyPressed)
{
    Character* mainCharacter { party->getMainCharacter() };
    
    // 主人公の向きを変更
    if (!isDashKeyPressed) {
        mainCharacter->setDirection(direction);
    }
    
    // 主人公が移動中でなければ
    if (!mainCharacter->isMoving()) {
        // 一定時間後に歩行開始
        if (!_task->isScheduled(PlayerControlTask::START_WALKING_SCHEDULE_KEY)) _task->scheduleOnce([this, party](float _) {
            _task->move(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
        }, 0.0f, PlayerControlTask::START_WALKING_SCHEDULE_KEY);
    }
}

// 決定キーが押された時
void BattleState::onEnterKeyPressed(Party* party)
{
    Character* mainCharacter { party->getMainCharacter() };
    if (mainCharacter->isInAttackMotion()) return;
    
    AttackCommand* command { AttackCommand::create() };
    command->setName("attack");
    command->setStamina(DungeonSceneManager::getInstance()->getStamina());
    command->setCallback([this, party](Character* c) {
        if (!BattleManager::getInstance()->isInBattle()) return;
        if (_task) _task->move(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
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
    
    if (step) {
        StepCommand* command { StepCommand::create() };
        command->setDirections(directions);
        command->setStamina(DungeonSceneManager::getInstance()->getStamina());
        command->setCallback([this, party](bool moved) {
            if (_task) _task->onPartyMovedOneGrid(party, false);
        });
        
        mainCharacter->pushCommand(command);
    } else {
        party->move(moveDirections, 1.f, [this, party, stamina](bool moved) {
            if (!moved) return;
            
            stamina->setDecreasing(false);
            if (_task) _task->onPartyMovedOneGrid(party, false);
        });
    }
}
