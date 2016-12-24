//
//  BattleCharacterData.h
//  6chefs2
//
//  Created by Ryoya Ino on 2016/11/21.
//
//

#ifndef BattleCharacterData_h
#define BattleCharacterData_h

#include "define.h"

class AssertHelper;
class AttackData;

class BattleCharacterData: public Ref
{
public:
    static const char* HIT_POINT;
    static const char* SPEED_RATIO;
    static const char* ATTACKS;
    static const char* STEP_INTERVAL_TIME;
    static const char* STEP_STAMINA;
    static const char* POWER;
    static const char* STAMINA;
    static const char* INTERVAL_TIME;
    static const char* MOTION_SOUND;
    static const char* MOTION_SOUND_VOLUME;
    static const char* HIT_SOUND;
    static const char* HIT_SOUND_VOLUME;
    
private:
    static rapidjson::Document BATTLE_CHARACTER_DATA;

private:
    AssertHelper* _assertHelper {nullptr};
    string _charaId {};
    map<string, AttackData*> _attacks {};
    int _hitPoint {100};
    float _speedRatio {1.0f};
    float _stepStamina {5.0f};
    float _stepIntervalTime {0.5f};
    
private:
    BattleCharacterData() {FUNCLOG};
    ~BattleCharacterData();
    bool init(const string& charaId);
    bool init(const int charaId);
    bool setHitPoint(const rapidjson::Value& battleCharacterData);
    bool setSpeedRatio(const rapidjson::Value& battleCharacterData);
    bool setAttacks(const rapidjson::Value& battleCharacterData);
    bool setStepStamina(const rapidjson::Value& battleCharacterData);
    bool setStepIntervalTime(const rapidjson::Value& battleCharacterData);

public:
    CREATE_FUNC_WITH_PARAM(BattleCharacterData, const string&);
    CREATE_FUNC_WITH_PARAM(BattleCharacterData, const int);
    int getHitPoint() const;
    float getSpeedRatio() const;
    AttackData* getAttackData(const string& attackName) const;
    float getStepIntervalTime() const;
    float getStepStamina() const;
};

#endif /* BattleCharacterData_h */
