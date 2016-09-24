//
//  EventScriptValidator.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/29.
//
//

#include "Event/EventScriptValidator.h"

#include "Utils/JsonUtils.h"
#include "Utils/AssertUtils.h"

const char* EventScriptValidator::REQUIRE {"require"};
const char* EventScriptValidator::MEMBER {"member"};

rapidjson::Document EventScriptValidator::validateConfig { rapidjson::Document() };

EventScriptValidator::EventScriptValidator(rapidjson::Value& targetEvent):targetEvent(targetEvent)
{
    FUNCLOG
}

bool EventScriptValidator::init()
{
    if (EventScriptValidator::validateConfig == rapidjson::Document()) {
        string path = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::EVENT_SCRIPT_VALIDATOR);
        if (path == "") return false;
        EventScriptValidator::validateConfig = LastSupper::JsonUtils::readJsonFile(path);
    }
    return true;
}

bool EventScriptValidator::validate()
{
    if (!this->targetEvent.HasMember("type")) {}
    if (!this->targetEvent["type"].IsString()) {}
    this->type = this->targetEvent["type"].GetString();
    cout << "validate >> " << this->type << endl;
    
    if (!this->validateConfig.HasMember(type.c_str())) {}
    
    rapidjson::Value& rule = this->validateConfig[type.c_str()];
    return this->checkObject(rule);
}

bool EventScriptValidator::checkObject(rapidjson::Value& rule)
{
    if (rule.HasMember(EventScriptValidator::REQUIRE)) {
        rapidjson::Value& require = rule[EventScriptValidator::REQUIRE];
        bool isOr = require[0].IsArray();
        bool isOk = false;
        if (isOr) {
            isOk = this->checkRequireOr(require);
        } else {
            isOk = this->checkRequire(require);
        }
        if (!isOk) {
            LastSupper::AssertUtils::fatalAssert("Event::" + this->type);
        }
    }
    return true;
}

#pragma mark -
#pragma mark CheckRequire

bool EventScriptValidator::checkRequireOr(rapidjson::Value& requireDoubleArray)
{
    int doubleArraySize = requireDoubleArray.Size();
    for (int i = 0; i < doubleArraySize; i++) {
        if (this->checkRequire(requireDoubleArray[i])) return true;
    }
    return false;
}

bool EventScriptValidator::checkRequire(rapidjson::Value& requireArray)
{
    int requireSize = requireArray.Size();
    for (int i = 0; i < requireSize; i++) {
        if (!this->checkRequireChild(requireArray[i])) {
            return false;
        }
    }
    return true;
}

bool EventScriptValidator::checkRequireChild(rapidjson::Value& target)
{
    cout << target.GetString() << endl;
    if (!this->targetEvent.HasMember(target.GetString())) return false;
    
    return true;
}

#pragma mark -
