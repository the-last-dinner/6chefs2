//
//  MovePattern.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/16.
//
//

#include "MapObjects/MovePatterns/MovePattern.h"

#include "MapObjects/Character.h"

#include "MapObjects/MapObjectList.h"

#include "MapObjects/Party.h"

// コンストラクタ
MovePattern::MovePattern() {};

// デストラクタ
MovePattern::~MovePattern() {};

// 初期化
bool MovePattern::init(Character* chara)
{
    if(!chara) return false;
    
    this->chara = chara;
    
    return true;
}

// 動き開始
void MovePattern::start()
{
    this->setPaused(false);
}

// 止めるフラグを変更
void MovePattern::setPaused(bool paused)
{
    this->paused = paused;
}

// 速度の倍率を設定
void MovePattern::setSpeedRatio(float ratio)
{
    this->speedRatio = ratio;
}

// 最初に通る経路オブジェクトのIDを取得
void MovePattern::setStartPathId(const int pathId)
{
    this->startPathId = pathId;
}

// 主人公を取得
Character* MovePattern::getMainCharacter() const { return this->chara->objectList->getParty()->getMainCharacter(); }

// マップオブジェクト一覧を取得
MapObjectList* MovePattern::getMapObjectList() const { return this->chara->objectList; }

// 停止中か
bool MovePattern::isPaused() const
{
    return this->paused;
}
