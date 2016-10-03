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
    CC_SAFE_RELEASE_NULL(_commandInProgress);
}

// 初期化
bool MapObjectCommandQueue::init()
{
    return true;
}

#pragma mark -
#pragma mark Queue

// 後ろにプッシュ
void MapObjectCommandQueue::push(MapObjectCommand* command)
{
    CC_SAFE_RETAIN(command);
    _commandQueue.pushBack(command);
}

// 先頭が実行可能か
bool MapObjectCommandQueue::isFrontExecutable(MapObject* mapObject) const
{
    if (_commandQueue.size() == 0) return false;
    
    MapObjectCommand* front { _commandQueue.front() };
    
    if (!front) return false;
    
    return front->isExecutable(mapObject);
}

// 先頭をポップ
MapObjectCommand* MapObjectCommandQueue::pop()
{
    if (_commandQueue.size() == 0) return nullptr;
    
    MapObjectCommand* front { _commandQueue.front() };
    
    _commandQueue.erase(0);
    
    return front;
}

// キューをクリア
void MapObjectCommandQueue::clear()
{
    _commandQueue.clear();
}

#pragma mark -
#pragma mark Update

void MapObjectCommandQueue::update(MapObject* mapObject, float delta)
{
    // 実行中のコマンドがあれば何もしない
    if (_commandInProgress && !_commandInProgress->isDone()) return;
    
    // 実行していたコマンドを解放
    CC_SAFE_RELEASE_NULL(_commandInProgress);
    
    // キューの先頭が実行可能でなければ無視
    if (!this->isFrontExecutable(mapObject)) return;
    
    MapObjectCommand* command { this->pop() };
    
    // コマンドがキューになければ何もしない
    if (!command) return;
    
    // コマンドを実行
    command->execute(mapObject);
    _commandInProgress = command;
}
