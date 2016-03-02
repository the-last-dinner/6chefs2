//
//  TerrainObject.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#include "MapObjects/TerrainObject/TerrainObject.h"

#include "MapObjects/Character.h"

// コンストラクタ
TerrainObject::TerrainObject() {};

// デストラクタ
TerrainObject::~TerrainObject() {};

int TerrainObject::getStampingState(Character* target) const
{
    return target->stampingState;
}

void TerrainObject::setStampingState(Character* target, int state)
{
    target->stampingState = state;
}

string TerrainObject::getPrefix(Character* target) const
{
    return target->texturePrefix;
}

// 足踏みする時
void TerrainObject::onWillStamp(Character* target, const Direction& direction, const float ratio)
{
    Animation* anime = AnimationCache::getInstance()->getAnimation(this->getPrefix(target) + to_string(etoi(direction)) + to_string(this->getStampingState(target) < 2 ? 0 : 1));
    this->setStampingState(target, this->getStampingState(target) + 1);
    if(this->getStampingState(target) > 3) this->setStampingState(target, 0);
    anime->setDelayPerUnit(DURATION_MOVE_ONE_GRID / ratio);
    
    target->getSprite()->runAction(Animate::create(anime));
    target->getSprite()->runAction(Sequence::createWithTwoActions(DelayTime::create(DURATION_MOVE_ONE_GRID / ratio), CallFunc::create([target]{target->setDirection(target->getDirection());})));
}