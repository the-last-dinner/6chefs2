//
//  BattleBoss.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/11.
//
//

#include "MapObjects/MovePatterns/BattleBoss.h"

#include "Effects/DustEffect.h"

#include "MapObjects/Character.h"
#include "MapObjects/Command/AttackCommand.h"
#include "MapObjects/Command/FastMoveCommand.h"
#include "MapObjects/Command/WalkCommand.h"
#include "MapObjects/DetectionBox/CollisionBox.h"
#include "MapObjects/DetectionBox/CollisionDetector.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/MovePatterns/Chaser.h"

#include "Managers/DungeonSceneManager.h"

// 定数
const string BattleBoss::FORWARD_ATTACK_NAME { "attack_1" };
const string BattleBoss::SPIN_ATTACK_NAME { "attack_2" };
const string BattleBoss::FLASH_ATTACK_NAME { "attack_3" };
const float BattleBoss::RANDOM_MOVE_RATIO { 0.3f };
const float BattleBoss::SPIN_ATTACK_RATIO { 0.3f };

// コンストラクタ
BattleBoss::BattleBoss() { FUNCLOG }

// デストラクタ
BattleBoss::~BattleBoss() { FUNCLOG }

// 初期化
bool BattleBoss::init(Character* character)
{
    if (!MovePattern::init(character)) return false;
    
    _subPattern = Chaser::create(character);
    CC_SAFE_RETAIN(_subPattern);
    _subPattern->setSpeedRatio(0.4f);
    
    return true;
}

#pragma mark -
#pragma mark Callback

void BattleBoss::onForwardAttackCommandFinished(Character* character)
{
    if (this->isPaused()) return;
    
    if (rand_0_1() <= RANDOM_MOVE_RATIO) {
        WalkCommand* command1 { WalkCommand::create() };
        WalkCommand* command2 { WalkCommand::create() };
        int idx { random(0, int(Direction::getAll().size()) - 1) };
        for (int i { 0 }; i < Direction::getAll().size(); i++) {
            if (idx != i) continue;
            command1->setDirection(Direction::getAll()[i]);
            command2->setDirection(Direction::getAll()[i]);
        }
        command1->setSpeed(0.8f);
        command2->setSpeed(0.2f);
        command2->setWalkCallback([this](bool walked){ this->start(); });
        character->pushCommand(command1);
        character->pushCommand(command2);
        return;
    }
    
    if (rand_0_1() <= SPIN_ATTACK_RATIO) {
        AttackCommand* command { AttackCommand::create() };
        command->setName(SPIN_ATTACK_NAME);
        command->setCallback(CC_CALLBACK_1(BattleBoss::onSpinAttackCommandFinished, this));
        character->pushCommand(command);
        return;
    }
    
    this->start();
}

void BattleBoss::onSpinAttackCommandFinished(Character* character)
{
    if (rand_0_1() <= RANDOM_MOVE_RATIO) {
        WalkCommand* command1 { WalkCommand::create() };
        WalkCommand* command2 { WalkCommand::create() };
        int idx { random(0, int(Direction::getAll().size()) - 1) };
        for (int i { 0 }; i < Direction::getAll().size(); i++) {
            if (idx != i) continue;
            command1->setDirection(Direction::getAll()[i]);
            command2->setDirection(Direction::getAll()[i]);
        }
        command1->setSpeed(0.8f);
        command2->setSpeed(0.2f);
        command2->setWalkCallback([this](bool walked){ this->start(); });
        character->pushCommand(command1);
        character->pushCommand(command2);
        return;
    }
    
    this->start();
}

void BattleBoss::onFastMoveCommandFinished()
{
    _chara->clearCommandQueue();
    AttackCommand* command { AttackCommand::create() };
    command->setName(FORWARD_ATTACK_NAME);
    command->setCallback(CC_CALLBACK_1(BattleBoss::onForwardAttackCommandFinished, this));
    _chara->pushCommand(command);
}

#pragma mark -
#pragma mark Interface

void BattleBoss::start()
{
    MovePattern::start();
    if (_subPattern) _subPattern->start();
}

void BattleBoss::pause()
{
    MovePattern::pause();
    if (_subPattern) _subPattern->pause();
}

void BattleBoss::resume()
{
    MovePattern::resume();
    if (_subPattern) _subPattern->resume();
}

void BattleBoss::onPartyMoved()
{
    if (_subPattern) _subPattern->onPartyMoved();
}

bool BattleBoss::canGoToNextMap() const
{
    return false;
}

void BattleBoss::update(float delta)
{
    if (_paused) return;
    if (!_chara) return;
    if (_chara->isInAttackMotion()) return;
    if (!_chara->getBattle()) return;
    
    Point forwardGridPos1 { _chara->getGridPosition() + _chara->getDirection().getGridVec2() };
    Point forwardGridPos2 { _chara->getGridPosition() + _chara->getDirection().getGridVec2() * 2 };
    
    if (_chara->getCollision()->intersectsGrid(this->getMainCharacter()->getCollision(), forwardGridPos1) ||
        _chara->getCollision()->intersectsGrid(this->getMainCharacter()->getCollision(), forwardGridPos2)) {
        _chara->clearCommandQueue();
        AttackCommand* command { AttackCommand::create() };
        command->setName(FORWARD_ATTACK_NAME);
        command->setCallback(CC_CALLBACK_1(BattleBoss::onForwardAttackCommandFinished, this));
        _chara->pushCommand(command);
        return;
    }
    
    Point charaGridPos { _chara->getGridPosition() };
    Point targetGridPos { this->getMainCharacter()->getGridPosition() };
    vector<Direction> directions { Direction::convertGridVec2(targetGridPos - charaGridPos) };
    bool existsCollisionBetween { this->getMapObjectList()->getCollisionDetector()->existsCollisionBetween(_chara, this->getMainCharacter()) };
    
    if (existsCollisionBetween) return;
    if (directions.size() == 1 && directions.front().getInt() == _chara->getDirection().getInt()) {
        _chara->clearCommandQueue();
        Direction direction { _chara->getDirection().getOppositeDirection() };
        Point gridPos { this->getMainCharacter()->getGridPosition() + direction.getGridVec2() * 2 };
        
        FastMoveCommand* command { FastMoveCommand::create() };
        command->setGridPosition(gridPos);
        command->setDelayTime(0.4f);
        command->setCallback(CC_CALLBACK_0(BattleBoss::onFastMoveCommandFinished, this));
        _chara->pushCommand(command);
        return;
    }
}
