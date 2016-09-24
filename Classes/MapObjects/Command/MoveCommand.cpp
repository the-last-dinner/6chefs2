//
//  MoveCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#include "MapObjects/Command/MoveCommand.h"

#include "MapObjects/MapObject.h"

// コンストラクタ
MoveCommand::MoveCommand() {}

// デストラクタ
MoveCommand::~MoveCommand() {}

// 初期化
bool MoveCommand::init()
{
    if(!MapObjectCommand::init()) return false;
    
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

void MoveCommand::setMoveCallcback(function<void()> callback)
{
    _onMoved = callback;
}

#pragma mark -
#pragma mark Interface

// 実行
void MoveCommand::execute(MapObject* target, function<void()> callback)
{
    target->moveBy(_directions, [this, callback] {
        if (_onMoved) _onMoved();
        if (callback) callback();
    }, _speed);
}

