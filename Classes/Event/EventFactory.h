//
//  EventFactory.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/24.
//
//

#ifndef __LastSupper__EventFactory__
#define __LastSupper__EventFactory__

#include "Common.h"

class GameEvent;
class EventSpawn;

class EventFactory : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC(EventFactory)

// インスタンスメソッド
private:
    EventFactory();
    ~EventFactory();
    bool init();
public:
    GameEvent* createGameEvent(rapidjson::Value& json);
    Vector<GameEvent*> createEventVector(rapidjson::Value& json);
    queue<GameEvent*> createEventQueue(rapidjson::Value& json);
};

#endif /* defined(__LastSupper__EventFactory__) */
