//
//  FastMoveCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/22.
//
//

#include "MapObjects/Command/FastMoveCommand.h"

#include "Effects/DustEffect.h"

#include "MapObjects/MapObject.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
FastMoveCommand::FastMoveCommand() { FUNCLOG }

// デストラクタ
FastMoveCommand::~FastMoveCommand() { FUNCLOG }

// 初期化
bool FastMoveCommand::init()
{
    if (!MapObjectCommand::init()) return false;
    return true;
}

#pragma mark -
#pragma mark Setter

void FastMoveCommand::setGridPosition(const Point& gridPosition)
{
    _gridPosition = gridPosition;
}

void FastMoveCommand::setDelayTime(float delayTime)
{
    _delayTime = delayTime;
}

void FastMoveCommand::setCallback(function<void()> callback)
{
    _callback = callback;
}

#pragma mark -
#pragma mark Interface

bool FastMoveCommand::isExecutable(MapObject* target) const
{
    return !target->isMoving();
}

void FastMoveCommand::execute(MapObject* target)
{
    DustEffect* effect { DustEffect::create() };
    effect->setPosition(target->getPosition());
    target->getParent()->addChild(effect);
    effect->animate([this, target](DustEffect* e){ target->getParent()->removeChild(e); });
    
    target->setVisible(false);
    target->enableHit(false);
    target->runAction(Sequence::createWithTwoActions(DelayTime::create(_delayTime), CallFunc::create([this, target] {
        target->setGridPosition(_gridPosition);
        DungeonSceneManager::getInstance()->setMapObjectPosition(target);
        target->setVisible(true);
        target->enableHit(true);
        
        this->setDone();
        if (_callback) _callback();
    })));
}
