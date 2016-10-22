//
//  EventQueue.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/10/16.
//
//

#include "Event/EventQueue.h"

// コンストラクタ
EventQueue::EventQueue() { FUNCLOG }

// デストラクタ
EventQueue::~EventQueue()
{
    FUNCLOG
    
    _queue.clear();
}

// 初期化
bool EventQueue::init()
{
    return true;
}

bool EventQueue::isEmpty() const
{
    return _queue.empty();
}

void EventQueue::pushBack(GameEvent* event)
{
    _queue.pushBack(event);
}

GameEvent* EventQueue::popFront()
{
    if (_queue.empty()) return nullptr;
    
    GameEvent* event { _queue.front() };
    
    _queue.erase(0);
    
    return event;
}
