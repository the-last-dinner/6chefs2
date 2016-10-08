//
//  GameEventHelper.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/24.
//
//

#ifndef __LastSupper__GameEventHelper__
#define __LastSupper__GameEventHelper__

#include "Common.h"

class MapObject;
class GameEvent;
class EventFactory;

class GameEventHelper : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC(GameEventHelper)
    
// インスタンスメソッド
private:
    GameEventHelper();
    ~GameEventHelper();
    bool init();
    
    // condition
    bool detectEquipFlg(rapidjson::Value& json, bool negative);
    bool detectEventFlg(rapidjson::Value& json, bool negative);
    bool detectFlg(rapidjson::Value& json, bool negative);
    bool detectItemFlg(rapidjson::Value& json, bool negative);
    bool detectStatusFlg(rapidjson::Value& json, bool negative);
    bool detectTrophyFlg(rapidjson::Value& json, bool negative);
    
public:
    bool hasMember(rapidjson::Value& json, const char* member) const;
    bool detectCondition(rapidjson::Value& json);
    
    MapObject* getMapObjectById(const string& objectId, bool available = true);
    template<typename T>
    inline T getMapObjectById(const string& objectId, bool available = true) { return dynamic_cast<T>(this->getMapObjectById(objectId, available)); };
    
    Point getPoint(rapidjson::Value& json);
    Point getToPoint(rapidjson::Value& json);
    Direction getDirection(rapidjson::Value& json);
    EnemyMovePattern getMovePatternForEnemy(rapidjson::Value& json);
    CharacterMovePattern getMovePatternForCharacter(rapidjson::Value& json);
    Trigger getTrigger(rapidjson::Value& json);
    int getEventId(rapidjson::Value& json);
    Color3B getColor(rapidjson::Value& json) const;
    
    GameEvent* createMiniGameSuccessCallbackEvent(rapidjson::Value& json, EventFactory* factory, GameEvent* caller);
    GameEvent* createMiniGameFailureCallbackEvent(rapidjson::Value& json, EventFactory* factory, GameEvent* caller);
};
#endif /* defined(__LastSupper__GameEventHelper__) */
