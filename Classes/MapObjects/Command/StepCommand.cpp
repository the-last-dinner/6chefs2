//
//  StepCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/03.
//
//

#include "MapObjects/Command/StepCommand.h"

#include "MapObjects/Character.h"

// コンストラクタ
StepCommand::StepCommand() { FUNCLOG }

// デストラクタ
StepCommand::~StepCommand() { FUNCLOG }

// 初期化
bool StepCommand::init()
{
    if (!MapObjectCommand::init()) return false;
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
    
    character->walkBy(_directions, [this](bool walked) {
        if (_callback) _callback(walked);
        this->setDone();
    }, _speed, false, false);
}
