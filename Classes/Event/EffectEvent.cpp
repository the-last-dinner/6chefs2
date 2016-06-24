//
//  EffectEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#include "Event/EffectEvent.h"

#include "Event/EventScriptMember.h"
#include "Event/EventScriptValidator.h"

#include "Effects/AmbientLightLayer.h"
#include "Effects/Fog.h"

#include "Managers/DungeonSceneManager.h"

#include "Scenes/DungeonScene.h"

#pragma mark SetAmbientLightEvent

bool SetAmbientLightEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    if(!this->validator->hasMember(json, member::AMBIENT)) return false;
    
    // 環境光の色
    map<string, Color3B> strToColor
    {
        {"basement", AmbientLightLayer::BASEMENT},
        {"day", AmbientLightLayer::DAY},
        {"evening", AmbientLightLayer::EVENING},
        {"night", AmbientLightLayer::NIGHT},
        {"midnight", AmbientLightLayer::MIDNIGHT},
        {"room", AmbientLightLayer::ROOM},
    };
    
    string str {json[member::AMBIENT].GetString()};
    
    if(strToColor.count(str) == 0) return false;
    
    this->color = strToColor[str];
    
    return true;
}

void SetAmbientLightEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getAmbientLayer()->setAmbient(this->color);
}

#pragma mark -
#pragma mark AnimationEvent

bool AnimationEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // img
    if(!validator->hasMember(json, member::IMGS)) return false;
    
    // 配列じゃなければ無視
    if(!json[member::IMGS].IsArray()) return false;
    
    for(int i {0}; i < json[member::IMGS].Size(); i++)
    {
        this->spriteFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(json[member::IMGS][i].GetString()));
    }
    
    // 一枚あたりの表示する時間
    if(this->validator->hasMember(json, member::TIME)) this->delayPerUnit = json[member::TIME].GetDouble();
    
    return true;
}

void AnimationEvent::run()
{
    Animation* animation { Animation::createWithSpriteFrames(this->spriteFrames) };
    
    animation->setDelayPerUnit(this->delayPerUnit);
    
    Sprite* animationSprite { Sprite::createWithSpriteFrame(this->spriteFrames.at(0)) };
    animationSprite->setPosition(WINDOW_CENTER);
    
    DungeonSceneManager::getInstance()->getScene()->addChild(animationSprite, Priority::TOP_COVER);
    
    animationSprite->runAction(Sequence::create(Animate::create(animation), CallFunc::create([this]{this->setDone();}), RemoveSelf::create(), nullptr));
}

#pragma mark -
#pragma mark CreateFogEvent

bool CreateFogEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    return true;
}

void CreateFogEvent::run()
{
    this->setDone();
    
    Fog* fog { Fog::create() };
    fog->setPosition(WINDOW_CENTER);
    
    DungeonSceneManager::getInstance()->getScene()->addChild(fog);
}

#pragma mark -
#pragma mark CreateRainEvent

bool CreateRainEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    return true;
}

void CreateRainEvent::run()
{
    this->setDone();
    
    Size size = Director::getInstance()->getVisibleSize();
    ParticleSystemQuad* rain = ParticleSystemQuad::create("img/rain.plist");
    rain->setPosition(Vec2(size.width/2,size.height));

    DungeonSceneManager::getInstance()->getScene()->addChild(rain);
}

#pragma mark -
#pragma mark CreateUnderwaterEvent

bool CreateUnderwaterEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    return true;
}

void CreateUnderwaterEvent::run()
{
    this->setDone();
    
    auto waterLayer = LayerColor::create(Color4B(88,255,255,128 ));
    ParticleSystemQuad* bubble = ParticleSystemQuad::create("img/bubble.plist");
    
    waterLayer->addChild(bubble);
    DungeonSceneManager::getInstance()->getScene()->addChild(waterLayer);
}


