//
//  MapObjectEvent.cpp
//  LastSupper
//
//  Created by Kohei on 2015/10/25.
//
//

#include "MapObjectEvent.h"

#include "Effects/Light.h"

#include "Event/GameEventHelper.h"
#include "Event/EventScriptMember.h"

#include "MapObjects/MapObject.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/Command/MoveCommand.h"
#include "MapObjects/Command/SetPositionCommand.h"
#include "Mapobjects/Party.h"
#include "MapObjects/PathFinder/PathFinder.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark MapObjectEvent

bool MapObjectEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // オブジェクトのIDを保持
    if (!_eventHelper->hasMember(_json, member::OBJECT_ID)) return false;
    
    _objectId = _json[member::OBJECT_ID].GetString();
    
    return true;
}

#pragma mark -
#pragma mark ReactionEvent

bool ReactionEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    return true;
}

void ReactionEvent::run()
{
    MapObject* target { _eventHelper->getMapObjectById(_objectId) };
    
    if (!target) {
        this->setDone();
        return;
    }

    target->reaction([this]{this->setDone();});
}

#pragma mark -
#pragma mark CreateMapObjectEvent

bool CreateMapObjectEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::CHARA_ID)) {
        // キャラの時
        _target = {_eventHelper->getMapObjectById(_objectId, false)};
        CC_SAFE_RETAIN(_target);
    } else {
        // キャラじゃない時
        // データ生成
        CharacterData data;
        data.obj_id = stoi(_objectId);
        data.chara_id = stoi(_json[member::CHARA_ID].GetString());
        data.location.x = _json[member::X].GetInt();
        data.location.y = _json[member::Y].GetInt();
        
        Direction direction {_eventHelper->getDirection(_json)};
        if (direction.isNull()) direction = Direction::DOWN;
        data.location.direction = direction;
        data.move_pattern = _eventHelper->getMovePatternForCharacter(_json);
        
        // データからキャラクタを生成
        Character* chara { Character::create(data) };
        CC_SAFE_RETAIN(chara);
        
        if (!chara) return nullptr;
        
        chara->setTrigger(_eventHelper->getTrigger(_json));
        chara->setEventId(_eventHelper->getEventId(_json));
        
        _target = chara;
    }
 
    return true;
}

void CreateMapObjectEvent::run()
{
    if (!_target) {
        this->setDone();
        return;
    }
    
    DungeonSceneManager::getInstance()->addMapObject(_target);
    CC_SAFE_RELEASE_NULL(_target);
    this->setDone();
}

#pragma mark -
#pragma mark RemoveMapObjectEvent

bool RemoveMapObjectEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    return true;
}

void RemoveMapObjectEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getMapObjectList()->removeById(stoi(_objectId));
}

#pragma mark -
#pragma mark FollowCharacterEvent

bool FollowCharacterEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    return true;
}

void FollowCharacterEvent::run()
{
    this->setDone();
    MapObject* follower {_eventHelper->getMapObjectById(_objectId)};
    DungeonSceneManager::getInstance()->getParty()->addMember(static_cast<Character*>(follower));
}

#pragma mark -
#pragma mark ReleaseFollowingCharacterEvent

bool ReleaseFollowingCharacterEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    return true;
}

void ReleaseFollowingCharacterEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getParty()->removeMember(stoi(_objectId));
}

#pragma mark -
#pragma mark WarpMapObjectEvents

bool WarpMapObjectEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::X)) return false;
    if (!_eventHelper->hasMember(_json, member::Y)) return false;
    if (!_eventHelper->hasMember(_json, member::DIRECTION)) return false;
    _point = Point(_json[member::X].GetInt(), _json[member::Y].GetInt());
    _direction = _eventHelper->getDirection(_json);
    
    return true;
}

void WarpMapObjectEvent::run()
{
    this->setDone();
    MapObject* target { _eventHelper->getMapObjectById(_objectId) };
    if (!target) return;
    
    target->getActionManager()->resumeTarget(target);
    target->clearCommandQueue();
    
    SetPositionCommand* command { SetPositionCommand::create() };
    command->setGridPosition(_point);
    command->setDirection(_direction);
    
    target->pushCommand(command);
}

#pragma mark -
#pragma mark MoveToEvent

bool MoveToEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    // 目的地
    _dest = _eventHelper->getPoint(_json);
    
    // 速さ倍率
    if (_eventHelper->hasMember(_json, member::SPEED)) _speedRatio = _json[member::SPEED].GetDouble();
    
    return true;
}

void MoveToEvent::run()
{
    MapObject* target { _eventHelper->getMapObjectById(_objectId) };
    
    if (!target) {
        this->setDone();
        return;
    }
    
    // 経路探索開始
    PathFinder* pathFinder { DungeonSceneManager::getInstance()->getMapObjectList()->getPathFinder() };
    deque<Direction> directions { pathFinder->getPath(target, _dest) };
    
    Vector<MoveCommand*> commands { MoveCommand::create(directions, [this](bool moved) { this->setDone(); }, _speedRatio) };
    
    for (MoveCommand* command : commands) {
        target->pushCommand(command);
    }
}

#pragma mark -
#pragma mark MoveByEvent

bool MoveByEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    // 向き
    _direction = _eventHelper->getDirection(_json);
    
    // 格子数
    _gridNum = static_cast<int>(json[member::STEPS].GetDouble() * 2);
    
    if (_direction.isNull() || _gridNum == 0) return false;
    
    // 速さ倍率
    if (_eventHelper->hasMember(_json, member::SPEED)) _speedRatio = _json[member::SPEED].GetDouble();
    
    return true;
}

void MoveByEvent::run()
{
    MapObject* target { _eventHelper->getMapObjectById(_objectId) };
    
    if (!target) {
        this->setDone();
        return;
    }
    
    Vector<MoveCommand*> commands { MoveCommand::create({ _direction }, _gridNum, [this](bool moved) { this->setDone(); }, _speedRatio) };
    
    for (MoveCommand* command : commands) {
        target->pushCommand(command);
    }
}

#pragma mark -
#pragma mark SetLightEvent

bool SetLightEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    Light::Type type = Light::Type::TORCH;
    
    Light::Information info {Light::TYPE_TO_INFO.at(type)};
    
    int range {1};
    
    // 範囲
    if (_eventHelper->hasMember(_json, member::RANGE)) range = _json[member::RANGE].GetInt();
    info.radius = range * GRID;
    
    // 色
    if (_eventHelper->hasMember(_json, member::COLOR)) info.color = _eventHelper->getColor(_json);
    
    // 方向
    if (_eventHelper->hasMember(_json, member::DIRECTION)) info.angle = _eventHelper->getDirection(_json).getAngle();
    
    // 現在のマップオブジェクトの方向があれば上書きする
    Direction direction {_eventHelper->getMapObjectById(_objectId)->getDirection()};
    if (!direction.isNull()) info.angle = direction.getAngle();
    
    // 光生成
    Light* innerLight { Light::create(info) };
    innerLight->setOpacity(0);
    innerLight->setBlendFunc(BlendFunc{ GL_SRC_ALPHA, GL_ONE });
    CC_SAFE_RETAIN(innerLight);
    _innerLight = innerLight;
    
    // 外側の光
    Color3B color { Color3B(info.color.r * 1.3f, info.color.g * 1.3f, info.color.b * 1.3f)};
    float radius {info.radius * 7.0f};
    
    if (_eventHelper->hasMember(_json, member::OPTION)) {
        string option = _json[member::OPTION].GetString();
        if (option == "flashlight") {
            type = Light::Type::FLASHLIGHT;
        }
    }
    Light::Information outerInfo {Light::TYPE_TO_INFO.at(type)};
    outerInfo.color = color;
    outerInfo.radius = radius;
    outerInfo.angle = info.angle;
    
    Light* outerLight {Light::create(outerInfo)};
    outerLight->setPosition(innerLight->convertToWorldSpace(innerLight->getPosition()));
    outerLight->setOpacity(0);
    outerLight->setBlendFunc(BlendFunc{GL_SRC_COLOR, GL_ONE});
    CC_SAFE_RETAIN(outerLight);
    
    _outerLight = outerLight;
    
    return true;
}

void SetLightEvent::run()
{
    MapObject* target { _eventHelper->getMapObjectById(_objectId) };
    
    target->setLight(_innerLight, _outerLight, DungeonSceneManager::getInstance()->getAmbientLayer(), [this]{this->setDone();});
    
    CC_SAFE_RELEASE_NULL(_innerLight);
    CC_SAFE_RELEASE_NULL(_outerLight);
}

#pragma mark -
#pragma mark RemoveLightEvent

bool RemoveLightEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    return true;
}

void RemoveLightEvent::run()
{
    MapObject* target { _eventHelper->getMapObjectById(_objectId) };
    
    target->removeLight([this]{this->setDone();});
}

#pragma mark -
#pragma mark SetMovableEvent

bool SetMovableEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::MOVABLE)) return false;
    
    _movable = _json[member::MOVABLE].GetBool();
    
    if (_eventHelper->hasMember(_json, member::FILE)) _fileName = _json[member::FILE].GetString();
    
    return true;
}

void SetMovableEvent::run()
{
    MapObject* target { _eventHelper->getMapObjectById(_objectId) };
    
    target->setMovable(_movable);
    if(_fileName != "") target->setMovingSoundFileName(_fileName);
    this->setDone();
}

#pragma mark -
#pragma mark MapObjectAnimateEvent

bool MapObjectAnimateEvent::init(rapidjson::Value& json)
{
    if (!MapObjectEvent::init(json)) return false;
    if (!_eventHelper->hasMember(_json, member::NAME)) return false;
    
    _animationName = _json[member::NAME].GetString();
    if (_eventHelper->hasMember(_json, member::SPEED)) _speed = static_cast<float>(_json[member::SPEED].GetDouble());
    
    return true;
}

void MapObjectAnimateEvent::run()
{
    MapObject* target { _eventHelper->getMapObjectById(_objectId) };
    target->resumeAnimation();
    target->playAnimation(_animationName, [this](MapObject* obj){ this->setDone(); });
}
