//
//  EventQueue.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/10/16.
//
//

#ifndef EventQueue_h
#define EventQueue_h

#include "Event/GameEvent.h"

class EventQueue : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC(EventQueue)

// インスタンス変数
private:
    Vector<GameEvent*> _queue {};
    
// インスタンスメソッド
private:
    EventQueue();
    ~EventQueue();
    bool init();
public:
    bool isEmpty() const;
    void pushBack(GameEvent* event);
    GameEvent* popFront();
};

#endif /* EventQueue_h */
