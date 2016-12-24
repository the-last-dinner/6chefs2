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
    
    _chara = chara;
    
    return true;
}

// 動き開始
void MovePattern::start()
{
    _started = true;
}

// 一時停止
void MovePattern::pause()
{
    _paused = true;
}

// 再開
void MovePattern::resume()
{
    _paused = false;
}

// 速度の倍率を設定
void MovePattern::setSpeedRatio(float ratio)
{
    _speedRatio = ratio;
}

// 最初に通る経路オブジェクトのIDを取得
void MovePattern::setStartPathId(const int pathId)
{
    _startPathId = pathId;
}

// 主人公を取得
Character* MovePattern::getMainCharacter() const { return _chara->_objectList->getParty()->getMainCharacter(); }

// マップオブジェクト一覧を取得
MapObjectList* MovePattern::getMapObjectList() const { return _chara->_objectList; }

// 停止中か
bool MovePattern::isPaused() const
{
    return _paused;
}

bool MovePattern::hasSterted() const
{
    return _started;
}

#pragma mark -
#pragma mark Interface

void MovePattern::update(float delta)
{
}
