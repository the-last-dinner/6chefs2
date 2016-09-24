//
//  MapObjectCommandQueue.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#include "MapObjects/Command/MapObjectCommandQueue.h"

// コンストラクタ
MapObjectCommandQueue::MapObjectCommandQueue() {}

// デストラクタ
MapObjectCommandQueue::~MapObjectCommandQueue()
{
    _commandQueue.clear();
}

// 初期化
bool MapObjectCommandQueue::init()
{
    return true;
}

// 後ろにプッシュ
void MapObjectCommandQueue::push(MapObjectCommand* command)
{
    _commandQueue.pushBack(command);
}

// 先頭をポップ
MapObjectCommand* MapObjectCommandQueue::pop()
{
    MapObjectCommand* first { _commandQueue.front() };
    
    _commandQueue.erase(0);
    
    return first;
}
