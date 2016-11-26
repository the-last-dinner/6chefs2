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

class AssertHelper;

class EventScriptValidator : public Ref
{
public:
    CREATE_FUNC_WITH_TWO_PARAM(EventScriptValidator, const string&, const string&);
private:
    EventScriptValidator() { FUNCLOG };
    ~EventScriptValidator();
    
// instance valiables
private:
    map<string, function<bool(const rapidjson::Value&, const rapidjson::Value&)>> _typeToValidateFunc;
    AssertHelper* _assertHelper;

private:
    static const string TYPE;
    static const string REQUIRE;
    static const string MEMBER;
    static const string MEMBERS;
    static const string ENUMS;
    static const string STRING;
    static const string RECURSIVE_OBJECT;
    static const string OBJECT;
    static const string RECURSIVE_ARRAY;
    static const string ARRAY;
    static const string NORMAL;
    static const string INT;
    static const string DOUBLE;
    static const string STOI;
    static const string ENUM;
    static const string BOOL;
    static rapidjson::Document VALIDATE_CONFIG;
    static const map<rapidjson::Type, string> TYPE_TO_VALIDATE_STRING;
    
// instance methods
public:
    bool init(const string& mapName, const string& eventName);
    bool validate(const rapidjson::Value& targetEvent);
private:
    // CheckObject
    bool checkObject(const rapidjson::Value& targetEvent, const rapidjson::Value& rule);
    
    // CheckRequire
    bool checkRequire(const rapidjson::Value& targetEvent, const rapidjson::Value& requires);
    bool checkRequireAnd(const rapidjson::Value& targetEvent, const rapidjson::Value& requireArray);
    bool checkRequireOr(const rapidjson::Value& targetEvent, const rapidjson::Value& requireDoubleArray);
    bool checkRequireChild(const rapidjson::Value& targetEvent, const rapidjson::Value& targetMemberName);
    
    // CheckMember
    bool checkMember(const rapidjson::Value& targetEvent, const rapidjson::Value& members);
    bool checkMemberType(const rapidjson::Value& targetMember, const rapidjson::Value& member);
    
    // CheckArray
    bool checkArray(const rapidjson::Value& targetMember, const rapidjson::Value& checkTypes);
    bool checkArraySimpleContents(const rapidjson::Value& targetMember, const rapidjson::Value& checkType);
    bool checkArrayMultipleContents(const rapidjson::Value& targetMember, const rapidjson::Value& checkTypes);
    
    // CheckType
    bool isObject(const rapidjson::Value& target, const rapidjson::Value& empty = rapidjson::Value());
    bool isArray(const rapidjson::Value& target, const rapidjson::Value& empty = rapidjson::Value());
    bool isInt(const rapidjson::Value& target, const rapidjson::Value& empty = rapidjson::Value());
    bool isDouble(const rapidjson::Value& target, const rapidjson::Value& empty = rapidjson::Value());
    bool isString(const rapidjson::Value& target, const rapidjson::Value& empty = rapidjson::Value());
    bool isStoi(const rapidjson::Value& target, const rapidjson::Value& empty = rapidjson::Value());
    bool isEnum(const rapidjson::Value& target, const rapidjson::Value& enumName);
    bool isBool(const rapidjson::Value& target, const rapidjson::Value& empty = rapidjson::Value());
    
    // Utils
    string getValidateFuncKey(const rapidjson::Value& checkType);
};

#endif /* EventScriptValidator_hpp */
