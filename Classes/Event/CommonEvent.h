//
//  CommonEvent.hpp
//  6chefs2
//
//  Created by S.inami on 2016/03/06.
//
//

#ifndef CommonEvent_h
#define CommonEvent_h

#include "Event/GameEvent.h"

// システムメッセージイベント
class CommonEvent :public GameEvent
{
    // インスタンスメソッド
public:
    CREATE_FUNC_WITH_PARAM(CommonEvent, rapidjson::Value&)
private:
    CommonEvent() {FUNCLOG};
    ~CommonEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
    int functionId;
};

#endif /* CommonEvent_h */
