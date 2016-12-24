//
//  SetPositionCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/24.
//
//

#include "MapObjects/Command/SetPositionCommand.h"

#include "MapObjects/MapObject.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
SetPositionCommand::SetPositionCommand() { FUNCLOG }

// デストラクタ
SetPositionCommand::~SetPositionCommand() { FUNCLOG }

// 初期化
bool SetPositionCommand::init()
{
    if (!MapObjectCommand::init()) return false;
    return true;
}

#pragma mark -
#pragma mark Setter

void SetPositionCommand::setGridPosition(const Point& gridPosition)
{
    _gridPosition = gridPosition;
}

void SetPositionCommand::setDirection(const Direction& direction)
{
    _direction = direction;
}

#pragma mark -
#pragma mark Interface

bool SetPositionCommand::isExecutable(MapObject* target) const
{
    return !target->isMoving();
}

void SetPositionCommand::execute(MapObject* target)
{
    target->setGridPosition(_gridPosition);
    target->setDirection(_direction);
    DungeonSceneManager::getInstance()->setMapObjectPosition(target);
    
    this->setDone();
}
