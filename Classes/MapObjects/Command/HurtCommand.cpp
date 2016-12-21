//
//  HurtCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/12.
//
//

#include "MapObjects/Command/HurtCommand.h"

#include "MapObjects/Character.h"

// コンストラクタ
HurtCommand::HurtCommand() { FUNCLOG }

// デストラクタ
HurtCommand::~HurtCommand() { FUNCLOG }

// 初期化
bool HurtCommand::init()
{
    if (!MapObjectCommand::init()) return false;
    return true;
}

#pragma mark -
#pragma mark Interface

bool HurtCommand::isExecutable(MapObject* target) const
{
    return true;
}

void HurtCommand::execute(MapObject* target)
{
    Character* character { dynamic_cast<Character*>(target) };
    
    if (!character) return;
    
    character->enableHit(false);
    this->setDone();
    
    character->runAction(Sequence::create(Hide::create(),
                                          DelayTime::create(0.15f),
                                          Show::create(),
                                          DelayTime::create(0.15f),
                                          Hide::create(),
                                          DelayTime::create(0.15f),
                                          Show::create(),
                                          DelayTime::create(0.15f),
                                          Hide::create(),
                                          DelayTime::create(0.15f),
                                          Show::create(),
                                          CallFunc::create([character, this] { character->enableHit(true);}),
                                          nullptr));
}
