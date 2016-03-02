//
//  MapObjectEvent.cpp
//  LastSupper
//
//  Created by Kohei on 2015/10/25.
//
//

#include "MapObjectEvent.h"

#include "Algorithm/PathFinder.h"

#include "Effects/Light.h"

#include "Event/EventScriptValidator.h"
#include "Event/EventScriptMember.h"

#include "MapObjects/MapObject.h"
#include "MapObjects/MapObjectList.h"
#include "Mapobjects/Party.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark MapObjectEvent

bool MapObjectEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // オブジェクトのIDを保持
    if (!this->validator->hasMember(json, member::OBJECT_ID)) return false;
    
    this->objectId = json[member::OBJECT_ID].GetString();
    
    return true;
}

#pragma mark -
#pragma mark ReactionEvent

bool ReactionEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
    
    return true;
}

void ReactionEvent::run()
{
    MapObject* target {this->validator->getMapObjectById(this->objectId)};
    
    if(!target)
    {
        this->setDone();
        
        return;
    }

    SoundManager::getInstance()->playSE(Resource::SE::REACTION);
    target->reaction([this]{this->setDone();});
}

#pragma mark -
#pragma mark CreateMapObjectEvent

bool CreateMapObjectEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
 
    return true;
}

void CreateMapObjectEvent::run()
{
    MapObject* target {this->validator->getMapObjectById(this->objectId, false)};
    
    if(!target)
    {
        this->setDone();
        
        return;
    }

    DungeonSceneManager::getInstance()->addMapObject(target);
    this->setDone();
}

#pragma mark -
#pragma mark RemoveMapObjectEvent

bool RemoveMapObjectEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
    
    return true;
}

void RemoveMapObjectEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getMapObjectList()->removeById(stoi(this->objectId));
}

#pragma mark -
#pragma mark FollowCharacterEvent

bool FollowCharacterEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
    
    return true;
}

void FollowCharacterEvent::run()
{
    this->setDone();
    MapObject* follower {this->validator->getMapObjectById(this->objectId)};
    DungeonSceneManager::getInstance()->getParty()->addMember(static_cast<Character*>(follower));
}

#pragma mark -
#pragma mark ReleaseFollowingCharacterEvent

bool ReleaseFollowingCharacterEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
    
    return true;
}

void ReleaseFollowingCharacterEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getParty()->removeMember(stoi(this->objectId));
}

#pragma mark -
#pragma mark WarpMapObjectEvents

bool WarpMapObjectEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
    
    if(!this->validator->hasMember(json, member::X)) return false;
    if(!this->validator->hasMember(json, member::Y)) return false;
    if(!this->validator->hasMember(json, member::DIRECTION)) return false;
    this->point = Point(json[member::X].GetInt(), json[member::Y].GetInt());
    this->direction = this->validator->getDirection(json);
    
    return true;
}

void WarpMapObjectEvent::run()
{
    this->setDone();
    MapObject* target { this->validator->getMapObjectById(this->objectId) };
    target->setGridPosition(this->point);
    target->setDirection(this->direction);
    DungeonSceneManager::getInstance()->setMapObjectPosition(target);
}

#pragma mark -
#pragma mark MoveToEvent

bool MoveToEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
    
    // 目的地
    this->dest = this->validator->getPoint(json);
    
    // 速さ倍率
    if(this->validator->hasMember(json, member::SPEED)) this->speedRatio = json[member::SPEED].GetDouble();
    
    return true;
}

void MoveToEvent::run()
{
    MapObject* target {this->validator->getMapObjectById(this->objectId)};
    
    if(!target)
    {
        this->setDone();
        
        return;
    }
    
    // 経路探索開始
    PathFinder* pathFinder { PathFinder::create(DungeonSceneManager::getInstance()->getMapSize()) };
    deque<Direction> directions { pathFinder->getPath(target->getGridRect(), target->getWorldGridCollisionRects(), this->dest) };
    
    target->moveByQueue(directions, [this](bool reached){this->setDone();}, this->speedRatio);
}

#pragma mark -
#pragma mark MoveByEvent

bool MoveByEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
    
    // 向き
    this->direction = this->validator->getDirection(json);
    
    // 格子数
    this->gridNum = static_cast<int>(json[member::STEPS].GetDouble() * 2);
    
    if(this->direction == Direction::SIZE || this->gridNum == 0) return false;
    
    // 速さ倍率
    if(this->validator->hasMember(json, member::SPEED)) this->speedRatio = json[member::SPEED].GetDouble();
    
    return true;
}

void MoveByEvent::run()
{
    MapObject* target { this->validator->getMapObjectById(this->objectId) };
    
    if(!target)
    {
        this->setDone();
        
        return;
    }
    
    target->moveBy(this->direction, this->gridNum, [this](bool _){this->setDone();}, this->speedRatio);
}

#pragma mark -
#pragma mark SetLightEvent

bool SetLightEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
    
    Light::Information info {};
    
    int range {1};
    
    // 範囲
    if(this->validator->hasMember(json, member::RANGE)) range = json[member::RANGE].GetInt();
    info.radius = range * GRID;
    
    // 色
    if(this->validator->hasMember(json, member::COLOR)) info.color = this->validator->getColor(json);
    
    // 光生成
    Light* light { Light::create(info) };
    CC_SAFE_RETAIN(light);
    this->light = light;
    
    return true;
}

void SetLightEvent::run()
{
    MapObject* target { this->validator->getMapObjectById(this->objectId) };
    
    target->setLight(this->light, DungeonSceneManager::getInstance()->getAmbientLayer(), [this]{this->setDone();});
    
    CC_SAFE_RELEASE_NULL(this->light);
}

#pragma mark -
#pragma mark RemvoeLightEvent

bool RemoveLightEvent::init(rapidjson::Value& json)
{
    if(!MapObjectEvent::init(json)) return false;
    
    return true;
}

void RemoveLightEvent::run()
{
    MapObject* target { this->validator->getMapObjectById(this->objectId) };
    
    target->removeLight([this]{this->setDone();});
}
