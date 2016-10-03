//
//  PathObject.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#include "MapObjects/PathObject.h"

// コンストラクタ
PathObject::PathObject() { FUNCLOG };

// デストラクタ
PathObject::~PathObject() { FUNCLOG };

// 初期化
bool PathObject::init()
{
    if(!MapObject::init()) return false;
    
    return true;
}

// 経路オブジェクトIDを設定
void PathObject::setPathId(int pathId)
{
    this->pathId = pathId;
}

// 前のオブジェクトIDを設定
void PathObject::setPreviousId(int previousId)
{
    this->previousId = previousId;
}

// 次のオブジェクトIDを設定
void PathObject::setNextId(int nextId)
{
    this->nextId = nextId;
}

// 周りを見渡すかどうか
void PathObject::setThrough(bool through)
{
    this->through = through;
}

// 次のオブジェクトまでの速度倍率を設定
void PathObject::setSpeedRatio(float speedRatio)
{
    this->speedRatio = speedRatio;
}

// 見渡す方向を設定
void PathObject::setLookDirection(const Direction& direction)
{
    this->direction = direction;
}

// 経路オブジェクトIDを取得
int PathObject::getPathId() const
{
    return this->pathId;
}

// 前のオブジェクトIDを取得
int PathObject::getPreviousId() const
{
    return this->previousId;
}

// 次のオブジェクトIDを取得
int PathObject::getNextId() const
{
    return this->nextId;
}

// 周りを見渡す必要があるか
bool PathObject::needsLookingAround() const
{
    return !this->through;
}

// 次のオブジェクトまでの速度倍率を取得
float PathObject::getSpeedRatio() const
{
    return this->speedRatio;
}

// 見渡す方向を取得
Direction PathObject::getLookDirection() const
{
    return this->direction;
}

