//
//  WalkCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#include "MapObjects/Command/WalkCommand.h"

#include "MapObjects/Character.h"

#pragma mark Create

Vector<WalkCommand*> WalkCommand::create(const vector<Direction>& directions, int gridNum, function<void(bool)> cb, float speed, bool back, bool ignoreCollision)
{
    Vector<WalkCommand*> commands {};
    
    if (directions.empty() || gridNum == 0) return commands;
    
    for (int i { 0 }; i < gridNum; i++) {
        WalkCommand* command { WalkCommand::create() };
        command->setDirections(directions);
        command->setSpeed(speed);
        command->setBack(back);
        command->setIgnoreCollision(ignoreCollision);
        command->setWalkCallback([cb](bool moved){ if (!moved && cb) cb(false); });
        
        commands.pushBack(command);
    }
    
    if (!commands.empty()) commands.back()->setWalkCallback(cb);
    
    return commands;
}

Vector<WalkCommand*> WalkCommand::create(const deque<Direction>& directions, function<void(bool)> cb, float speed, bool back, bool ignoreCollision)
{
    Vector<WalkCommand*> commands {};
    
    for (Direction direction : directions) {
        WalkCommand* command { WalkCommand::create() };
        command->setDirection(direction);
        command->setSpeed(speed);
        command->setBack(back);
        command->setIgnoreCollision(ignoreCollision);
        command->setWalkCallback([cb](bool moved){ if (!moved && cb) cb(false); });
        
        commands.pushBack(command);
    }
    
    if (!commands.empty()) commands.back()->setWalkCallback(cb);
    
    return commands;
}

// コンストラクタ
WalkCommand::WalkCommand() {}

// デストラクタ
WalkCommand::~WalkCommand() {}

// 初期化
bool WalkCommand::init()
{
    if(!MapObjectCommand::init()) return false;
    
    return true;
}

#pragma mark -
#pragma mark Setter

void WalkCommand::setDirection(const Direction& direction)
{
    _directions = { direction };
}

void WalkCommand::setDirections(const vector<Direction>& directions)
{
    _directions = directions;
}

void WalkCommand::setWalkCallback(function<void(bool)> callback)
{
    _callback = callback;
}

void WalkCommand::setSpeed(const float speed)
{
    _speed = speed;
}

void WalkCommand::setBack(const bool back)
{
    _back = back;
}

void WalkCommand::setIgnoreCollision(const bool ignore)
{
    _ignoreCollision = ignore;
}

#pragma mark -
#pragma mark Interface

// 実行可能状態か
bool WalkCommand::isExecutable(MapObject* target) const
{
    return !target->isMoving();
}

// 実行
void WalkCommand::execute(MapObject* target)
{
    Character* character { dynamic_cast<Character*>(target) };
    
    if (!character) return;
    
    function<void(bool)> callback = _callback;
    character->walkBy(_directions, [this, callback](bool walked) {
        if (callback) callback(walked);
        this->setDone();
    }, _speed, _back, _ignoreCollision);
}
