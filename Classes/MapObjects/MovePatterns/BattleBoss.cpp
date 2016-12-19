//
//  BattleBoss.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/11.
//
//

#include "MapObjects/MovePatterns/BattleBoss.h"

#include "MapObjects/Character.h"
#include "MapObjects/Command/AttackCommand.h"
#include "MapObjects/Command/WalkCommand.h"
#include "MapObjects/DetectionBox/CollisionBox.h"
#include "MapObjects/MovePatterns/Chaser.h"

// 定数
const string BattleBoss::FORWARD_ATTACK_NAME { "attack_1" };
const string BattleBoss::SPIN_ATTACK_NAME { "attack_2" };
const string BattleBoss::FLASH_ATTACK_NAME { "attack_3" };
const float BattleBoss::RANDOM_MOVE_RATIO { 0.3f };

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

void BattleBoss::onAttackCommandFinished(Character* character)
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
    
    this->start();
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
    
    if (!_chara->isMoving()) {
        _chara->setDirection(Direction::convertVec2(this->getMainCharacter()->getPosition() - _chara->getPosition()));
    }
    
    Point forwardGridPos1 { _chara->getGridPosition() + _chara->getDirection().getGridVec2() };
    Point forwardGridPos2 { _chara->getGridPosition() + _chara->getDirection().getGridVec2() * 2 };
    
    if (_chara->getCollision()->intersectsGrid(this->getMainCharacter()->getCollision(), forwardGridPos1) ||
        _chara->getCollision()->intersectsGrid(this->getMainCharacter()->getCollision(), forwardGridPos2)) {
        _chara->clearCommandQueue();
        AttackCommand* command { AttackCommand::create() };
        command->setName(FORWARD_ATTACK_NAME);
        command->setCallback(CC_CALLBACK_1(BattleBoss::onAttackCommandFinished, this));
        _chara->pushCommand(command);
    }
}
