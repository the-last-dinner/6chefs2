//
//  BattleEvent.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#include "Event/BattleEvent.h"

#pragma mark BattleStartEvent

bool BattleStartEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    return true;
}

void BattleStartEvent::run()
{
    
}
