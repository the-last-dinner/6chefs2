//
//  AttackCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/20.
//
//

#include "MapObjects/Command/AttackCommand.h"

#include "Datas/BattleCharacterData.h"
#include "Datas/AttackData.h"
#include "MapObjects/Character.h"
#include "MapObjects/DetectionBox/AttackBox.h"
#include "MapObjects/Status/Stamina.h"

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

// 操作対象のスタミナを設定
void AttackCommand::setStamina(Stamina* stamina)
{
    _stamina = stamina;
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
    
    AttackData* data { character->getBattleCharacterData()->getAttackData(_name) };
    
    character->setAttackHitCallback([this, character](MapObject* mo){ this->onAttackHitted(character, mo); });
    
    character->beInAttackMotion(true);

    if (!data) {
        LastSupper::AssertUtils::fatalAssert("AttackData is missing.\n You should check move pattern.\ncharaID: "
                                             + to_string(character->getCharacterId()) + "\nattackName: " + _name);
        return;
    }
    
    character->getBattleAttackBox()->setPower(data->power);
    character->playAnimation(Character::AnimationName::getAttack(_name, character->getDirection()), [character, this](MapObject* obj)
    {
        this->onAttackAnimationFinished(character);
    });
    
    if (data->motionSound != "") {
        SoundManager::getInstance()->playSE(data->motionSound, data->motionSoundVolume);
    }
    
    if (_stamina) {
        _stamina->decrease(data->stamina);
    }
}

#pragma mark -
#pragma mark Callback

// 攻撃モーション再生終了時
void AttackCommand::onAttackAnimationFinished(Character* character)
{
    AttackData* attackData { character->getBattleCharacterData()->getAttackData(_name) };
    character->runAction(Sequence::createWithTwoActions(DelayTime::create(attackData->intervalTime), CallFunc::create([this, character] {
        character->beInAttackMotion(false);
        character->clearCommandQueue();
        character->setAttackHitCallback(nullptr);
        this->setDone();
        if (_callback) {
            _callback(character);
        }
    })));
}

// 攻撃ヒット時
void AttackCommand::onAttackHitted(Character* character, MapObject* hittedObject)
{
    AttackData* data { character->getBattleCharacterData()->getAttackData(_name) };
    if (data->hitSound != "") {
        SoundManager::getInstance()->playSE(data->hitSound, data->hitSoundVolume);
    }
}
