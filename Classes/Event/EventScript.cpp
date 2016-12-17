//
//  EventScript.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/03/15.
//
//

#include "Event/EventScript.h"
#include "Utils/JsonUtils.h"

// コンストラクタ
EventScript::EventScript() {FUNCLOG}

// デストラクタ
EventScript::~EventScript() {FUNCLOG}

// 初期化
bool EventScript::init(const string& jsonFileName)
{
    _json = LastSupper::JsonUtils::readJsonCrypted(FileUtils::getInstance()->fullPathForFilename("event/" + jsonFileName + ES_EXTENSION));
    
    return true;
}

//リソースのプリロード関数
vector<string> EventScript::getPreLoadList(const string& type){
    vector<string> list {};
    const char* typec = type.c_str();
    if (!_json.HasMember(typec)) return list;
    rapidjson::Value& obj = _json[typec];
    SizeType len = obj.Size();
    for(int i=0;i<len;i++){
        list.push_back(obj[i].GetString());
    }
    return list;
}

// 該当idのスクリプトを取得
rapidjson::Value EventScript::getScriptJson(const int eventId)
{
    return getScriptJson(to_string(eventId).c_str());
}

rapidjson::Value EventScript::getScriptJson(const string& eventId)
{
    return getScriptJson(eventId.c_str());
}

rapidjson::Value EventScript::getScriptJson(const char* eventId)
{
    rapidjson::Value::MemberIterator itr { _json.FindMember(eventId) };
    
    if (itr == _json.MemberEnd()) {
        rapidjson::Value nullValue;
        return nullValue;
    }
    
    rapidjson::Value json {};
    json.CopyFrom(itr->value, _json.GetAllocator());
    
    return json;
}

rapidjson::Document EventScript::getDocument()
{
    rapidjson::Document document(&_json.GetAllocator());
    return document;
}
