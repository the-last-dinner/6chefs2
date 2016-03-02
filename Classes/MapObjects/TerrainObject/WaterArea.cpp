//
//  WaterArea.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#include "MapObjects/TerrainObject/WaterArea.h"

#include "MapObjects/Character.h"

// 定数
const float WaterArea::SPEED_RATIO { 0.25 };

// コンストラクタ
WaterArea::WaterArea() {FUNCLOG};

// デストラクタ
WaterArea::~WaterArea() {FUNCLOG};

// 初期化
bool WaterArea::init()
{
    return true;
}

// オブジェクトが動こうとした時
void WaterArea::onWillMove(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio)
{
    // 速度を1/2にして移動
    target->move(directions, onMoved, ratio * SPEED_RATIO);
}

// 足踏みする時
void WaterArea::onWillStamp(Character* target, const Direction& direction, const float ratio)
{
    Animation* anime = AnimationCache::getInstance()->getAnimation("swim_" + this->getPrefix(target) + to_string(etoi(direction)) + to_string(this->getStampingState(target) < 2 ? 0 : 1));
    this->setStampingState(target, this->getStampingState(target) + 1);
    if(this->getStampingState(target) > 3) this->setStampingState(target, 0);
    anime->setDelayPerUnit(DURATION_MOVE_ONE_GRID / ratio);
    
    target->getSprite()->runAction(Animate::create(anime));
    target->getSprite()->runAction(Sequence::createWithTwoActions(DelayTime::create(DURATION_MOVE_ONE_GRID / (ratio * SPEED_RATIO)), CallFunc::create([target]{target->setDirection(target->getDirection());})));
}
