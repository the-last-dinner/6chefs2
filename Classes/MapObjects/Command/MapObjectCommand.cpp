//
//  MapObjectCommand.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#include "MapObjects/Command/MapObjectCommand.h"

// コンストラクタ
MapObjectCommand::MapObjectCommand() {}

// デストラクタ
MapObjectCommand::~MapObjectCommand() {}

// 初期化
bool MapObjectCommand::init()
{
    return true;
}

// 終了フラグを立てる
void MapObjectCommand::setDone()
{
    _isDone = true;
}

// 終了判定
bool MapObjectCommand::isDone() const
{
    return _isDone;
}
