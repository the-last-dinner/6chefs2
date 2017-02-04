//
//  EffectEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#include "Event/EffectEvent.h"

#include "Event/EventScriptMember.h"
#include "Event/GameEventHelper.h"

#include "Effects/AmbientLightLayer.h"
#include "Effects/Fog.h"

#include "Managers/DungeonSceneManager.h"

#include "Scenes/DungeonScene.h"

#pragma mark SetAmbientLightEvent

bool SetAmbientLightEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::AMBIENT)) return false;
    
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
    
    string str { _json[member::AMBIENT].GetString() };
    
    if(strToColor.count(str) == 0)
    {
        LastSupper::AssertUtils::warningAssert("EventScriptError\ntype: setAmbient\nambient: " + str + "This ambient is missing.");
        return false;
    }
    
    _color = strToColor[str];
    
    return true;
}

void SetAmbientLightEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->getAmbientLayer()->setAmbient(_color);
}

#pragma mark -
#pragma mark AnimationEvent

bool AnimationEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // img
    if (!_eventHelper->hasMember(_json, member::IMGS)) return false;
    
    // 配列じゃなければ無視
    if (!_json[member::IMGS].IsArray()) return false;
    
    for (int i {0}; i < _json[member::IMGS].Size(); i++) {
        _spriteFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(_json[member::IMGS][i].GetString()));
    }
    
    // 一枚あたりの表示する時間
    if (_eventHelper->hasMember(_json, member::TIME)) _delayPerUnit = _json[member::TIME].GetDouble();
    
    return true;
}

void AnimationEvent::run()
{
    Animation* animation { Animation::createWithSpriteFrames(_spriteFrames) };
    
    animation->setDelayPerUnit(_delayPerUnit);
    
    Sprite* animationSprite { Sprite::createWithSpriteFrame(_spriteFrames.at(0)) };
    animationSprite->setPosition(WINDOW_CENTER);
    
    DungeonSceneManager::getInstance()->getScene()->addChild(animationSprite, Priority::TOP_COVER);
    
    animationSprite->runAction(Sequence::create(Animate::create(animation), CallFunc::create([this]{this->setDone();}), RemoveSelf::create(), nullptr));
}

#pragma mark -
#pragma mark CreateFogEvent

bool CreateFogEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
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
    if (!GameEvent::init(json)) return false;
    
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
    if (!GameEvent::init(json)) return false;
    
    if (_eventHelper->hasMember(_json, member::WAVE_IN)) {
        _waveIn = _json[member::WAVE_IN].GetBool();
    }
    
    return true;
}

void CreateUnderwaterEvent::run()
{
    this->setDone();
    
    auto waterLayer = LayerColor::create(Color4B(88,255,255,128));
    
    ParticleSystemQuad* bubble = ParticleSystemQuad::create("img/bubble.plist");
    waterLayer->addChild(bubble);
    
    auto nodeGrid = NodeGrid::create();
    nodeGrid->runAction(RepeatForever::create(Waves::create(10.f, Size(60, 60), 10, 5.0f, true, false)));
    nodeGrid->addChild(waterLayer);
    
    if (this->_waveIn) {
        float layerHeight { waterLayer->getContentSize().height };
        waterLayer->setPosition(waterLayer->getPosition().x, - layerHeight);
        waterLayer->runAction(
            Sequence::createWithTwoActions(
                EaseSineInOut::create(MoveBy::create(1.0f, Vec2(0.f, 2 * layerHeight / 10))),
                Repeat::create(
                    Sequence::createWithTwoActions(
                        EaseSineInOut::create(MoveBy::create(0.5f, Vec2(0.f, - layerHeight / 10))),
                        EaseSineInOut::create(MoveBy::create(1.0f, Vec2(0.f, 3 * layerHeight / 10)))
                ), 4)
            )
        );
    }
    
    DungeonSceneManager::getInstance()->getScene()->addChild(nodeGrid);
}


