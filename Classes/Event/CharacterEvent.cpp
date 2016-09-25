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

#include "Managers/DungeonSceneManager.h"

#pragma mark CharacterEvent

bool CharacterEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    if(!this->eventHelper->hasMember(json, member::OBJECT_ID)) return false;
    
    this->objectId = json[member::OBJECT_ID].GetString();
    
    return true;
}

bool CharacterEvent::onRun()
{
    Character* target { this->eventHelper->getMapObjectById<Character*>(this->objectId) };
    if(!target)
    {
        this->setDone();
        
        LastSupper::AssertUtils::fatalAssert("指定IDのキャラクターが存在しません" + string(member::OBJECT_ID) + " : " + this->objectId);
        
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
    
    this->direction = this->eventHelper->getDirection(json);
    
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
    
    this->direction = this->eventHelper->getDirection(json);
    
    this->gridNum = static_cast<int>(json[member::STEPS].GetDouble() * 2);
    
    if(this->direction.isNull() || this->gridNum == 0) return false;
    
    if(this->eventHelper->hasMember(json, member::SPEED)) this->speedRatio = json[member::SPEED].GetDouble();
    
    if(this->eventHelper->hasMember(json, member::OPTION)) this->back = true;
    
    return true;
}

void WalkByEvent::run()
{
    if(!CharacterEvent::onRun()) return;
    
    this->target->pauseAi();
    this->target->getActionManager()->resumeTarget(this->target);
    
    if(this->target->isPaused()) this->target->setPaused(false);
    
    Vector<WalkCommand*> commands { WalkCommand::create({ this->direction }, this->gridNum, [this](bool walked) {
        this->setDone();
    }, this->speedRatio, this->back, true) };
    
    for (WalkCommand* command : commands) {
        this->target->pushCommand(command);
    }
}

#pragma mark -
#pragma mark WalkToEvent

bool WalkToEvent::init(rapidjson::Value& json)
{
    if(!CharacterEvent::init(json)) return false;
    
    // 目的地座標をcocos座標系で保持
    this->destPosition = this->eventHelper->getPoint(json);
    
    // 速さの倍率
    if(this->eventHelper->hasMember(json, member::SPEED)) this->speedRatio = json[member::SPEED].GetDouble();
    
    return true;
}

void WalkToEvent::run()
{
    if(!CharacterEvent::onRun()) return;
    
    // NOTICE: イベントからの命令のみで動かしたいのでAIを一時停止
    this->target->pauseAi();
    this->target->getActionManager()->resumeTarget(this->target);
    
    if (this->target->isPaused()) this->target->setPaused(false);
    
    // 経路探索開始
    PathFinder* pathFinder { DungeonSceneManager::getInstance()->getMapObjectList()->getPathFinder() };
    deque<Direction> directions { pathFinder->getPath(this->target, this->destPosition) };
    
    Vector<WalkCommand*> commands { WalkCommand::create( directions, [this](bool walked) {
        this->setDone();
    }, this->speedRatio, false, true) };
    
    for (WalkCommand* command : commands) {
        this->target->pushCommand(command);
    }
}

#pragma mark -
#pragma mark ChangeHeroEvent

bool ChangeHeroEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init()) return false;
    if (!this->eventHelper->hasMember(json, member::CHARA_ID)) return false;
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
    Character* chara { Character::create(CharacterData(this->charaId, etoi(ObjectID::HERO), location)) };
    party->addMember(chara);
    DungeonSceneManager::getInstance()->getMapLayer()->setParty(party);
    DungeonSceneManager::getInstance()->setCamera(party->getMainCharacter());
}
