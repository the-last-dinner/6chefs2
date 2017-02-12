//
//  TrophyConfigData.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/12.
//
//

#ifndef TrophyConfigData_h
#define TrophyConfigData_h

#include "define.h"

class TrophyConfigData : public Ref
{
    // Class methods
public:
    CREATE_FUNC(TrophyConfigData)
private:
    TrophyConfigData(){FUNCLOG};
    ~TrophyConfigData(){FUNCLOG};
    
    // constants
public:
    static const char* FAST_CLEAR;
    static const char* NO_SAVE_CLEAR;
    static const char* CHIKEN_SAVE;
    static const char* TROPHY_COMPLETE;
    static const char* TROPHY_ID;
    static const char* TIME;
    static const char* COUNT;
    
    // instance valiables
private:
    rapidjson::Document trophyConfig {nullptr};
    string filePath {};
    
    // instance methods
    public:
    bool init();
    int getTrophyId(const char* targetTrophy);
    int getTrophyIntegerProperty(const char* targetTrophy, const char* targetProperty);
    rapidjson::Value& getTrophyData(const char* targetTrophy);
};

#endif /* TrophyConfigData_hpp */
