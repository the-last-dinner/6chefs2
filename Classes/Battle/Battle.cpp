//
//  Battle.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#include "Battle/Battle.h"

#include "Battle/BattleData.h"

#include "Event/GameEvent.h"

#include "Managers/BattleManager.h"
#include "Managers/DungeonSceneManager.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/Party.h"
#include "MapObjects/Status/HitPoint.h"

#include "Scenes/DungeonScene.h"

#include "Tasks/EventTask.h"

// コンストラクタ
Battle::Battle() { FUNCLOG }

// デストラクタ
Battle::~Battle()
{
    FUNCLOG
    CC_SAFE_RELEASE_NULL(_data);
}

// 初期化
bool Battle::init(BattleData* data, DungeonSceneManager* manager)
{
    if (!Node::init()) return false;
    if (!data) return false;
    
    CC_SAFE_RETAIN(data);
    _data = data;
    
    MapObjectList* objectList { manager->getMapObjectList() };
    
    for (int objectId : data->getTargetObjectIds()) {
        MapObject* object { objectList->getMapObject(objectId) };
        if (!object) continue;
        _targetObjects.pushBack(object);
        this->setLostHPCallback(object);
    }
    
    _mainCharacter = objectList->getParty()->getMainCharacter();
    _eventTask = manager->getEventTask();
    _scene = manager->getScene();
    _objectList = objectList;
    
    if (!_mainCharacter) return false;
    if (!_eventTask) return false;
    if (!_scene) return false;
    if (!_objectList) return false;
    
    this->setLostHPCallback(_mainCharacter);
    
    BattleManager::getInstance()->setBattleInstance(this);
    
    return true;
}

void Battle::setLostHPCallback(MapObject* target)
{
    target->setLostHPCallback([this](MapObject* obj) {
        obj->clearCommandQueue();
        obj->runAction(Sequence::create(FadeOut::create(1.f), CallFunc::create([this, obj] {
            if (_objectList) _objectList->removeEnemyByObjectId(obj->getObjectId());
            if (_mainCharacter == obj) {
                _mainCharacter = nullptr;
                return;
            }
            obj->setOpacity(255);
            _targetObjects.eraseObject(obj);
        }), nullptr));
    });
}

bool Battle::isAllTargetDestroyed() const
{
    return _targetObjects.empty();
}

bool Battle::isMainCharacterDestroyed() const
{
    return !_mainCharacter;
}

void Battle::onFinish()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(Battle::update));
    _eventTask->runEventQueue();
    _scene->onBattleFinished(this);
    if (_finishCallback) _finishCallback(this);
    _scene->onAllEnemyRemoved();
}

void Battle::start()
{
    _scene->onBattleStart(this);
    this->schedule(CC_SCHEDULE_SELECTOR(Battle::update), 1.f);
}

Vector<MapObject*> Battle::getTargetObjects() const
{
    return _targetObjects;
}

#pragma mark -
#pragma mark Interface
void Battle::update(float delta)
{
    if (this->isAllTargetDestroyed()) {
        CC_SAFE_RETAIN(_data->getSuccessCallbackEvent());
        _eventTask->pushEventBack(_data->getSuccessCallbackEvent());
        this->onFinish();
        return;
    }
    
    if (this->isMainCharacterDestroyed()) {
        CC_SAFE_RETAIN(_data->getFailureCallbackEvent());
        _eventTask->pushEventBack(_data->getFailureCallbackEvent());
        this->onFinish();
        return;
    }
}
