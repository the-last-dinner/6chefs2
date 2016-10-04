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
class EventScriptValidator;

class EventFactory : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC(EventFactory)

// インスタンス変数
private:
    static const map<string, function<GameEvent*(rapidjson::Value&)>> typeToCreateFunc;
    EventScriptValidator* validator {};
    
// インスタンスメソッド
private:
    EventFactory() { FUNCLOG };
    ~EventFactory() { FUNCLOG };
    bool init();
public:
    GameEvent* createGameEvent(rapidjson::Value& json);
};

#endif /* defined(__LastSupper__EventFactory__) */
