//
//  BattleEvent.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#ifndef BattleEvent_h
#define BattleEvent_h

#include "Event/GameEvent.h"

class BattleStartEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(BattleStartEvent, rapidjson::Value&)
private:
    BattleStartEvent() { FUNCLOG };
    ~BattleStartEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json) override;
    virtual void run() override;
};

#endif /* BattleEvent_h */
