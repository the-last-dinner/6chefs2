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

class BattleCharacterData: public Ref
{
private:
    static rapidjson::Document battleCharacterData;

private:
    string charaId {etos(CharacterID::UNDIFINED)};
    map<string, int> attacks {};
    int hitPoint {100};
    float speedRatio {1.0f};
    
private:
    BattleCharacterData() {FUNCLOG};
    ~BattleCharacterData() {FUNCLOG};

public:
    CREATE_FUNC_WITH_PARAM(BattleCharacterData, const string&);
    CREATE_FUNC_WITH_PARAM(BattleCharacterData, const int);
    bool init(const string& charaId);
    bool init(const int charaId);
};

#endif /* BattleCharacterData_h */
