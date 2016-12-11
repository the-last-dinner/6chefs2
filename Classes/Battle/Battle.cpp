//
//  Battle.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#include "Battle/Battle.h"

#include "Battle/BattleData.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/Party.h"
#include "MapObjects/Status/HitPoint.h"

// コンストラクタ
Battle::Battle() { FUNCLOG }

// デストラクタ
Battle::~Battle()
{
    FUNCLOG
    
    CC_SAFE_RELEASE_NULL(_data);
}

// 初期化
bool Battle::init(BattleData* data, MapObjectList* objectList)
{
    if (!Node::init()) return false;
    if (!data) return false;
    
    CC_SAFE_RETAIN(data);
    _data = data;
    
    for (int objectId : data->getTargetObjectIds()) {
        MapObject* object { objectList->getMapObject(objectId) };
        if (object) _targetObjects.pushBack(object);
    }
    
    _mainCharacter = objectList->getParty()->getMainCharacter();
    
    if (!_mainCharacter) return false;
    
    this->schedule(CC_SCHEDULE_SELECTOR(Battle::update), 0.5f);
    
    return true;
}

bool Battle::isAllTargetDestroyed() const
{
    for (MapObject* obj : _targetObjects) {
        if (!obj->getHitPoint()->isLost()) return false;
    }
    return true;
}

bool Battle::isMainCharacterDestroyed() const
{
    return _mainCharacter->getHitPoint()->isLost();
}

#pragma mark -
#pragma mark Interface
void Battle::update(float delta)
{
    this->isAllTargetDestroyed();
    this->isMainCharacterDestroyed();
}
