//
//  EventScriptValidator.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/29.
//
//

#ifndef EventScriptValidator_h
#define EventScriptValidator_h

#include "define.h"

class EventScriptValidator : public Ref
{
public:
    CREATE_FUNC_WITH_PARAM_CONSRUCT(EventScriptValidator, rapidjson::Value&);
private:
    EventScriptValidator(rapidjson::Value& targetEvent);
    ~EventScriptValidator() { FUNCLOG };
    
// instance valiables
private:
    rapidjson::Value& targetEvent;
    string type;

private:
    static const char* REQUIRE;
    static const char* MEMBER;
    static rapidjson::Document validateConfig;
    
// instance methods
public:
    bool init();
    bool validate();
private:
    bool checkObject(rapidjson::Value& rule);
    bool checkRequire(rapidjson::Value& requires);
    bool checkRequireOr(rapidjson::Value& requireArray);
    bool checkRequireChild(rapidjson::Value& require);
    bool checkGroup();
    bool isInt(rapidjson::Value& target);
    bool isString();
    bool isArray();
    bool isStoi();
};

#endif /* EventScriptValidator_hpp */
