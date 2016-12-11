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

class BattleCharacterData: public Ref
{
public:
    static const char* HIT_POINT;
    static const char* SPEED_RATIO;
    static const char* ATTACKS;
    
private:
    static rapidjson::Document BATTLE_CHARACTER_DATA;

private:
    AssertHelper* _assertHelper {nullptr};
    string _charaId {};
    map<string, int> _attacks {};
    int _hitPoint {100};
    float _speedRatio {1.0f};
    
private:
    BattleCharacterData() {FUNCLOG};
    ~BattleCharacterData() {FUNCLOG};
    bool init(const string& charaId);
    bool init(const int charaId);
    bool setHitPoint(const rapidjson::Value& battleCharacterData);
    bool setSpeedRatio(const rapidjson::Value& battleCharacterData);
    bool setAttacks(const rapidjson::Value& battleCharacterData);

public:
    CREATE_FUNC_WITH_PARAM(BattleCharacterData, const string&);
    CREATE_FUNC_WITH_PARAM(BattleCharacterData, const int);
    int getHitPoint() const;
    float getSpeedRatio() const;
    int getAttackPoint(const string& attackName) const;
};

#endif /* BattleCharacterData_h */
