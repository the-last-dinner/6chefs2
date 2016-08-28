//
//  EventScriptValidator.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/29.
//
//

#include "Event/EventScriptValidator.h"

#include "Utils/JsonUtils.h"

bool EventScriptValidator::init()
{
    string path = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::EVENT_SCRIPT_VALIDATOR);
    if (path == "") return false;
    this->validateConfig = LastSupper::JsonUtils::readJsonFile(path);
    return true;
}

bool EventScriptValidator::validate(const rapidjson::Value& json)
{
    if (!json.HasMember("type")) {}
    if (!json["type"].IsString()) {}
    string type = json["type"].GetString();
    return true;
}