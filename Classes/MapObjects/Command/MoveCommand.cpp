//
//  MoveCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#include "MapObjects/Command/MoveCommand.h"

#include "MapObjects/MapObject.h"

#pragma mark Create

Vector<MoveCommand*> MoveCommand::create(const vector<Direction>& directions, int gridNum, function<void(bool)> cb, float speed)
{
    Vector<MoveCommand*> commands {};
    
    if (directions.empty() || gridNum == 0) return commands;
    
    for (int i { 0 }; i < gridNum; i++) {
        MoveCommand* command { MoveCommand::create() };
        command->setDirections(directions);
        command->setSpeed(speed);
        command->setMoveCallback([cb](bool moved){ if (!moved && cb) cb(false); });
        
        commands.pushBack(command);
    }
    
    if (!commands.empty()) commands.back()->setMoveCallback(cb);
    
    return commands;
}

Vector<MoveCommand*> MoveCommand::create(const deque<Direction>& directions, function<void(bool)> cb, float speed)
{
    Vector<MoveCommand*> commands {};
    
    for (Direction direction : directions) {
        MoveCommand* command { MoveCommand::create() };
        command->setDirection(direction);
        command->setSpeed(speed);
        command->setMoveCallback([cb](bool moved){ if (!moved && cb) cb(false); });
        
        commands.pushBack(command);
    }
    
    if (!commands.empty()) commands.back()->setMoveCallback(cb);
    
    return commands;
}

// コンストラクタ
MoveCommand::MoveCommand() {}

// デストラクタ
MoveCommand::~MoveCommand() {}

// 初期化
bool MoveCommand::init()
{
    if (!MapObjectCommand::init()) return false;
    
    return true;
}

#pragma mark -
#pragma mark Setter

void MoveCommand::setDirection(const Direction& direction)
{
    _directions = { direction };
}

void MoveCommand::setDirections(const vector<Direction>& directions)
{
    _directions = directions;
}

void MoveCommand::setMoveCallback(function<void(bool)> callback)
{
    _callback = callback;
}

void MoveCommand::setSpeed(const float speed)
{
    _speed = speed;
}

void MoveCommand::setIgnoreCollision(const bool ignore)
{
    _ignoreCollision = ignore;
}

#pragma mark -
#pragma mark Interface

// 実行可能状態か
bool MoveCommand::isExecutable(MapObject* target) const
{
    return !target->isMoving();
}

// 実行
void MoveCommand::execute(MapObject* target)
{
    target->moveBy(_directions, [this](bool moved) {
        if (_callback) _callback(moved);
        this->setDone();
    }, _speed, _ignoreCollision);
}

