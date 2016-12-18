//
//  AttackCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/20.
//
//

#include "MapObjects/Command/AttackCommand.h"

#include "Datas/BattleCharacterData.h"
#include "MapObjects/Character.h"
#include "MapObjects/DetectionBox/AttackBox.h"

// コンストラクタ
AttackCommand::AttackCommand() { FUNCLOG }

// デストラクタ
AttackCommand::~AttackCommand() { FUNCLOG }

// 初期化
bool AttackCommand::init()
{
    if (!MapObjectCommand::init()) return false;
    
    return true;
}

// 攻撃モーション名を設定
void AttackCommand::setName(const string& name)
{
    _name = name;
}

// コールバックを設定
void AttackCommand::setCallback(function<void(Character*)> callback)
{
    _callback = callback;
}

#pragma mark -
#pragma mark Interface

// 実行可能か
bool AttackCommand::isExecutable(MapObject* target) const
{
    Character* character { dynamic_cast<Character*>(target) };
    if (!character) return false;
    return !character->isInAttackMotion();
}

// 実行
void AttackCommand::execute(MapObject* target)
{
    Character* character { dynamic_cast<Character*>(target) };
    if (!character || !character->getBattleAttackBox()) {
        this->setDone();
        return;
    }
    
    character->beInAttackMotion(true);
    character->getBattleAttackBox()->setPower(character->getBattleCharacterData()->getAttackPoint(_name));
    character->playAnimation(Character::AnimationName::getAttack(_name, character->getDirection()), CC_CALLBACK_1(AttackCommand::onAttackAnimationFinished, this));
}

#pragma mark -
#pragma mark Callback

// 攻撃モーション再生終了時
void AttackCommand::onAttackAnimationFinished(Character* character)
{
    character->beInAttackMotion(false);
    character->clearCommandQueue();
    this->setDone();
    if (_callback) {
        _callback(character);
    }
}

