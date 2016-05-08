//
//  CharacterEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/27.
//
//

#include "Event/CharacterEvent.h"

#include "Event/EventScriptValidator.h"
#include "Event/EventScriptMember.h"

#include "Algorithm/PathFinder.h"

#include "Layers/Dungeon/TiledMapLayer.h"

#include "MapObjects/Character.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/Party.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark CharacterEvent

bool CharacterEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    if(!this->validator->hasMember(json, member::OBJECT_ID)) return false;
    
    this->objectId = json[member::OBJECT_ID].GetString();
    
    return true;
}

bool CharacterEvent::onRun()
{
    Character* target { this->validator->getMapObjectById<Character*>(this->objectId) };
    if(!target)
    {
        this->setDone();
        
        return false;
    }
    
    this->target = target;
    
    return true;
}

#pragma mark -
#pragma mark ChangeDirectionEvent

bool ChangeDirectionEvent::init(rapidjson::Value& json)
{
    if(!CharacterEvent::init(json)) return false;
    
    this->direction = this->validator->getDirection(json);
    
    if(this->direction == Direction::SIZE) return false;
    
    return true;
}

void ChangeDirectionEvent::run()
{
    if(!CharacterEvent::onRun()) return;
    
    this->target->setDirection(this->direction);
    this->setDone();
}

#pragma mark -
#pragma mark WalkByEvent

bool WalkByEvent::init(rapidjson::Value& json)
{
    if(!CharacterEvent::init(json)) return false;
    
    this->direction = this->validator->getDirection(json);
    
    this->gridNum = static_cast<int>(json[member::STEPS].GetDouble() * 2);
    
    if(this->direction == Direction::SIZE || this->gridNum == 0) return false;
    
    if(this->validator->hasMember(json, member::SPEED)) this->speedRatio = json[member::SPEED].GetDouble();
    
    if(this->validator->hasMember(json, member::OPTION)) this->back = true;
    
    return true;
}

void WalkByEvent::run()
{
    if(!CharacterEvent::onRun()) return;

    this->target->getActionManager()->resumeTarget(this->target);
}

void WalkByEvent::update(float delta)
{
    if(this->target->isMoving() || this->isCommandSent) return;
    
    if(this->target->isPaused()) this->target->setPaused(false);
    
    this->target->walkBy(this->direction, this->gridNum, [this](bool _){this->setDone();}, this->speedRatio, this->back);
    
    this->isCommandSent = true;
}

#pragma mark -
#pragma mark WalkToEvent

bool WalkToEvent::init(rapidjson::Value& json)
{
    if(!CharacterEvent::init(json)) return false;
    
    // 目的地座標をcocos座標系で保持
    this->destPosition = this->validator->getPoint(json);
    
    // 速さの倍率
    if(this->validator->hasMember(json, member::SPEED)) this->speedRatio = json[member::SPEED].GetDouble();
    
    return true;
}

void WalkToEvent::run()
{
    if(!CharacterEvent::onRun()) return;

    this->target->getActionManager()->resumeTarget(this->target);
}

void WalkToEvent::update(float delta)
{
    if(this->target->isMoving() || this->isCommandSent) return;
    
    if(this->target->isPaused()) this->target->setPaused(false);
    
    // 経路探索開始
    PathFinder* pathFinder { PathFinder::create(DungeonSceneManager::getInstance()->getMapSize()) };
    deque<Direction> directions { pathFinder->getPath(this->target->getGridRect(), this->target->getWorldGridCollisionRects(), this->destPosition) };
    
    this->target->walkByQueue(directions, [this](bool reached){this->setDone();}, this->speedRatio);
    
    this->isCommandSent = true;
}

#pragma mark -
#pragma mark ChangeHeroEvent

bool ChangeHeroEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init()) return false;
    if (!this->validator->hasMember(json, member::CHARA_ID)) return false;
    this->charaId = stoi(json[member::CHARA_ID].GetString());
    
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
    Character* chara {Character::create(CharacterData(this->charaId, etoi(ObjectID::HERO), location))};
    chara->setHit(true);
    party->addMember(chara);
    DungeonSceneManager::getInstance()->getMapLayer()->setParty(party);
    
}