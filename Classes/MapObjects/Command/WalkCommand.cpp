//
//  WalkCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#include "MapObjects/Command/WalkCommand.h"

#include "MapObjects/Character.h"

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

void WalkCommand::setWalkCallback(function<void()> callback)
{
    _onWalked = callback;
}

void WalkCommand::setSpeed(const float speed)
{
    _speed = speed;
}

void WalkCommand::setBack(const bool back)
{
    _back = back;
}

#pragma mark -
#pragma mark Interface

// 実行
void WalkCommand::execute(MapObject* target, function<void()> callback)
{
    Character* character { dynamic_cast<Character*>(target) };
    
    if (!character) {
        if (callback) callback();
        return;
    }
    
    character->walkBy(_directions, [this, callback] {
        if (_onWalked) _onWalked();
        if (callback) callback();
    }, _speed, _back);
}
