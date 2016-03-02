//
//  EnemyEvent.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#ifndef EnemyEvent_h
#define EnemyEvent_h

#include "Event/GameEvent.h"

#include "Datas/MapObject/EnemyData.h"

// 敵生成イベント
class CreateEnemyEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(CreateEnemyEvent, rapidjson::Value&)
private:
    EnemyData data {};
private:
    CreateEnemyEvent() {FUNCLOG};
    ~CreateEnemyEvent() {FUNCLOG};
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

// 敵削除イベント
class RemoveEnemyEvent : public GameEvent
{
public:
    CREATE_FUNC_WITH_PARAM(RemoveEnemyEvent, rapidjson::Value&)
private:
    int enemyId { etoi(EnemyID::UNDIFINED) };
private:
    RemoveEnemyEvent() { FUNCLOG };
    ~RemoveEnemyEvent() { FUNCLOG };
    virtual bool init(rapidjson::Value& json);
    virtual void run() override;
};

#endif /* EnemyEvent_h */
