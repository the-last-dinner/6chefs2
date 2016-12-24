//
//  StepCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/03.
//
//

#include "MapObjects/Command/StepCommand.h"

#include "Datas/BattleCharacterData.h"

#include "MapObjects/Character.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/DetectionBox/AttackDetector.h"
#include "MapObjects/Status/Stamina.h"

#include "Effects/DustEffect.h"

// 定数
const int StepCommand::MOVE_GRID_NUM { 3 };

// コンストラクタ
StepCommand::StepCommand() { FUNCLOG }

// デストラクタ
StepCommand::~StepCommand() { FUNCLOG }

// 初期化
bool StepCommand::init()
{
    if (!MapObjectCommand::init()) return false;
    
    _restGridNum = MOVE_GRID_NUM;
    
    return true;
}

#pragma mark -
#pragma mark Setter

void StepCommand::setDirection(const Direction& direction)
{
    _directions = { direction };
}

void StepCommand::setDirections(const vector<Direction>& directions)
{
    _directions = directions;
}

void StepCommand::setSpeed(const float speed)
{
    _speed = speed;
}

void StepCommand::setCallback(function<void(bool)> callback)
{
    _callback = callback;
}

void StepCommand::setStamina(Stamina* stamina)
{
    _stamina = stamina;
}

#pragma mark -
#pragma mark Interface

bool StepCommand::isExecutable(MapObject* target) const
{
    return !target->isMoving();
}

void StepCommand::execute(MapObject* target)
{
    Character* character { dynamic_cast<Character*>(target) };
    if (!character) return;
    
    this->moveCharacter(character);
}

#pragma mark -
#pragma mark Helper

void StepCommand::moveCharacter(Character* character)
{
    _restGridNum--;
    
    character->enableHit(false);
    bool movable { character->walkBy(_directions, [this, character](bool moved) {
        if (_callback && _restGridNum == 0) {
            BattleCharacterData* data { character->getBattleCharacterData() };
            character->runAction(Sequence::createWithTwoActions(DelayTime::create(data->getStepIntervalTime()), CallFunc::create([this, character, moved] {
                character->enableHit(true);
                _callback(moved);
                this->setDone();
            })));
        } else if (_restGridNum > 0) {
            this->moveCharacter(character);
        }
    }, _speed * float(MOVE_GRID_NUM), true, false) };
    
    if (_restGridNum == MOVE_GRID_NUM - 1 && movable) {
        if (_stamina) {
            _stamina->decrease(character->getBattleCharacterData()->getStepStamina());
        }
        
        character->runAction(Sequence::createWithTwoActions(
            EaseQuadraticActionOut::create(MoveBy::create(MapObject::DURATION_MOVE_ONE_GRID / 2, Vec2(0, GRID))),
            EaseQuadraticActionIn::create(MoveBy::create(MapObject::DURATION_MOVE_ONE_GRID / 2, Vec2(0, -GRID)))
        ));
        
        DustEffect* effect { DustEffect::create() };
        effect->setPosition(character->getPosition());
        character->getParent()->addChild(effect);
        effect->animate([character](DustEffect* e){ character->getParent()->removeChild(e); });
    }
}
