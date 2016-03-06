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
    this->json = LastSupper::JsonUtils::readJsonCrypted(FileUtils::getInstance()->fullPathForFilename("event/" + jsonFileName + ES_EXTENSION));
    return true;
}

//リソースのプリロード関数
vector<string> EventScript::getPreLoadList(const string& type){
    vector<string> list {};
    const char* typec = type.c_str();
    if (!this->json.HasMember(typec)) return list;
    rapidjson::Value& obj = this->json[typec];
    SizeType len = obj.Size();
    for(int i=0;i<len;i++){
        list.push_back(obj[i].GetString());
    }
    return list;
}

// 該当idのスクリプトを取得
rapidjson::Value& EventScript::getScriptJson(const int eventId)
{
    return getScriptJson(to_string(eventId).c_str());
}

rapidjson::Value& EventScript::getScriptJson(const string& eventId)
{
    return getScriptJson(eventId.c_str());
}

rapidjson::Value& EventScript::getScriptJson(const char* eventId)
{
    rapidjson::Value::MemberIterator itr {this->json.FindMember(eventId)};
    if(itr == this->json.MemberEnd())
    {
        static rapidjson::Value nullValue;
        return nullValue;
    }
    return itr->value;
}