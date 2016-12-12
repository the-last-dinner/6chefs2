//
//  BattleData.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#include "Battle/BattleData.h"

#include "Event/GameEvent.h"

// コンストラクタ
BattleData::BattleData() { FUNCLOG }

// デストラクタ
BattleData::~BattleData()
{
    FUNCLOG
    
    CC_SAFE_RELEASE_NULL(_successCallbackEvent);
    CC_SAFE_RELEASE_NULL(_failureCallbackEvent);
}

// 初期化
bool BattleData::init()
{
    return true;
}

#pragma mark -
#pragma mark TargetObjectIds
void BattleData::setTargetObjectIds(const vector<int>& ids)
{
    _targetObjectIds = ids;
}

vector<int> BattleData::getTargetObjectIds() const
{
    return _targetObjectIds;
}


#pragma mark -
#pragma mark SuccessCallbackEvent
void BattleData::setSuccessCallbackEvent(GameEvent* event)
{
    CC_SAFE_RELEASE_NULL(_successCallbackEvent);
    CC_SAFE_RETAIN(event);
    _successCallbackEvent = event;
}

GameEvent* BattleData::getSuccessCallbackEvent() const
{
    return _successCallbackEvent;
}

#pragma mark -
#pragma mark FailureCallbackEvent
void BattleData::setFailureCallbackEvent(GameEvent* event)
{
    CC_SAFE_RELEASE_NULL(_failureCallbackEvent);
    CC_SAFE_RETAIN(event);
    _failureCallbackEvent = event;
}

GameEvent* BattleData::getFailureCallbackEvent() const
{
    return _failureCallbackEvent;
}
