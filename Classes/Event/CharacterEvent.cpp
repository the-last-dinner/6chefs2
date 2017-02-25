//
//  CharacterEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/27.
//
//

#include "Event/CharacterEvent.h"

#include "Event/GameEventHelper.h"
#include "Event/EventScriptMember.h"

#include "Layers/Dungeon/TiledMapLayer.h"

#include "MapObjects/Character.h"
#include "MapObjects/Command/WalkCommand.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/Party.h"
#include "MapObjects/PathFinder/PathFinder.h"

#include "Tasks/CameraTask.h"

#include "Managers/DungeonSceneManager.h"
#include "Models/EquipItemEvent.h"

#pragma mark CharacterEvent

bool CharacterEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::OBJECT_ID)) return false;
    
    _objectId = _json[member::OBJECT_ID].GetString();
    
    return true;
}

Character* CharacterEvent::getTargetByObjectId(const string &objectId)
{
    Character* target { _eventHelper->getMapObjectById<Character*>(_objectId) };
    if (!target) {
        LastSupper::AssertUtils::fatalAssert("指定IDのキャラクターが存在しません" + string(member::OBJECT_ID) + " : " + _objectId);
        return nullptr;
    }
    
    return target;
}

#pragma mark -
#pragma mark ChangeDirectionEvent

bool ChangeDirectionEvent::init(rapidjson::Value& json)
{
    if (!CharacterEvent::init(json)) return false;
    
    _direction = _eventHelper->getDirection(_json);
    
    return true;
}

void ChangeDirectionEvent::run()
{
    Character* target { this->getTargetByObjectId(_objectId) };
    
    if (!target) {
        this->setDone();
        return;
    }
    
    target->setDirection(_direction);
    this->setDone();
}

#pragma mark -
#pragma mark WalkByEvent

bool WalkByEvent::init(rapidjson::Value& json)
{
    if(!CharacterEvent::init(json)) return false;
    
    _direction = _eventHelper->getDirection(_json);
    
    _gridNum = static_cast<int>(_json[member::STEPS].GetDouble() * 2);
    
    if (_direction.isNull() || _gridNum == 0) return false;
    
    if (_eventHelper->hasMember(_json, member::SPEED)) _speedRatio = _json[member::SPEED].GetDouble();
    
    if (_eventHelper->hasMember(_json, member::OPTION)) _back = true;
    
    return true;
}

void WalkByEvent::run()
{
    Character* target { this->getTargetByObjectId(_objectId) };
    
    if (!target) {
        this->setDone();
        return;
    }
    
    target->pauseAi();
    target->resumeAnimation();
    
    if (target->isPaused()) target->setPaused(false);
    
    Vector<WalkCommand*> commands { WalkCommand::create({ _direction }, _gridNum, [this](bool walked) {
        this->setDone();
    }, _speedRatio, _back, true) };
    
    for (WalkCommand* command : commands) {
        target->pushCommand(command);
    }
}

#pragma mark -
#pragma mark WalkToEvent

bool WalkToEvent::init(rapidjson::Value& json)
{
    if (!CharacterEvent::init(json)) return false;
    
    // 目的地座標をcocos座標系で保持
    _destPosition = _eventHelper->getPoint(_json);
    
    // 速さの倍率
    if (_eventHelper->hasMember(_json, member::SPEED)) _speedRatio = _json[member::SPEED].GetDouble();
    
    return true;
}

void WalkToEvent::run()
{
    Character* target { this->getTargetByObjectId(_objectId) };
    
    if (!target) {
        this->setDone();
        return;
    }
    
    // NOTICE: イベントからの命令のみで動かしたいのでAIを一時停止
    target->pauseAi();
    target->resumeAnimation();
    
    if (target->isPaused()) target->setPaused(false);
    
    // 経路探索開始
    PathFinder* pathFinder { DungeonSceneManager::getInstance()->getMapObjectList()->getPathFinder() };
    deque<Direction> directions { pathFinder->getPath(target, _destPosition) };
    
    Vector<WalkCommand*> commands { WalkCommand::create( directions, [this](bool walked) {
        this->setDone();
    }, _speedRatio, false, true) };
    
    for (WalkCommand* command : commands) {
        target->pushCommand(command);
    }
}

#pragma mark -
#pragma mark ChangeHeroEvent

bool ChangeHeroEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::CHARA_ID)) return false;
    _charaId = stoi(_json[member::CHARA_ID].GetString());
    
    return true;
}

void ChangeHeroEvent::run()
{
    this->setDone();
    
    // 現在のパーティを取得
    Party* party {DungeonSceneManager::getInstance()->getMapObjectList()->getParty()};
    Location location {party->getMainCharacter()->getCharacterData().location};
    
    // パーティを解散
    party->removeMemberAll();
    DungeonSceneManager::getInstance()->getMapObjectList()->removeById(etoi(ObjectID::HERO));
    
    // 新主人公を設定
    Character* chara { Character::create(CharacterData(_charaId, etoi(ObjectID::HERO), location)) };
    party->addMember(chara);
    DungeonSceneManager::getInstance()->getMapLayer()->setParty(party);
    DungeonSceneManager::getInstance()->getCamera()->setTarget(party->getMainCharacter());
    
    // 装備データをセット
    LocalPlayerData* localPlayerData {PlayerDataManager::getInstance()->getLocalData()};
    DungeonSceneManager::getInstance()->getEquipItemEvent()->setEquipmentCache(
        localPlayerData->getItemEquipment(DirectionRight()),
        localPlayerData->getItemEquipment(DirectionLeft())
    );
 }

#pragma mark -
#pragma mark ChangeSpeedEvent

bool ChangeSpeedEvent::init(rapidjson::Value& json)
{
    if (!CharacterEvent::init(json)) return false;
    
    if (!json.HasMember(member::SPEED)) return false;
    
    _speed = _json[member::SPEED].GetDouble();
    
    return true;
}

void ChangeSpeedEvent::run()
{
    if (_objectId == "hero") {
        Vector<Character*> members { DungeonSceneManager::getInstance()->getMapObjectList()->getParty()->getMembers() };
        for (Character* chara : members) {
            chara->setSpeed(_speed);
        }
    } else {
        Character* target { _eventHelper->getMapObjectById<Character*>(_objectId) };
        target->setSpeed(_speed);
    }
    this->setDone();
}
