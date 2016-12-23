//
//  DebugConfigData.h
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/28.
//
//

#ifndef DebugConfigData_h
#define DebugConfigData_h

#include "define.h"

class DebugConfigData : public Ref
{
public:
    enum struct Keys
    {
        DEBUG_MASK,
        STATS,
        INVINCIBLE_MODE,
        PLAIN_DATA,
        CRYPT_TRIGGER,
    };
    
    // Class methods
public:
    CREATE_FUNC(DebugConfigData)
private:
    DebugConfigData(){FUNCLOG};
    ~DebugConfigData(){FUNCLOG};
    
    // constants
public:
    static const char* DEBUG_MASK;
    static const char* STATS;
    static const char* INVINCIBLE_MODE;

    // Instance valiables
private:
    string filePath {};
    bool hasDebugConfig {false};
    rapidjson::Document debugConfig {nullptr};
    
    // instance methods
public:
    bool init();
    bool getBoolValue(const char* key);
    bool isDebugMode();
private:
    void writeConfig();
};


#endif /* DebugConfigData_hpp */
