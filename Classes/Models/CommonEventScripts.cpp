//
//  CommonEventScripts.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/05/15.
//
//

#include "Event/EventScript.h"
#include "Models/CommonEventScripts.h"
#include "Utils/AssertUtils.h"
#include "Utils/JsonUtils.h"

const char* CommonEventScripts::NAME {"name"};
const char* CommonEventScripts::CHAPTER {"chapter"};
const char* CommonEventScripts::ALL {"all"};

#pragma mark init
// 初期化
bool CommonEventScripts::init()
{
    return this->getEventScriptsConfig();
}

// CommonEventScripts設定ファイル読み込み
bool CommonEventScripts::getEventScriptsConfig()
{
    string path = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::COMMON_EVENT);
    if (path == "") return false;
    this->config = LastSupper::JsonUtils::readJsonCrypted(path);
    return true;
}

#pragma mark -
#pragma mark control

// イベントスクリプトのロード
bool CommonEventScripts::loadEventScripts(const int chapter)
{
    this->releaseEventScripts();
    string path {};
    string fileName {};
    char chapChar[10];
    sprintf(chapChar, "%d", chapter);
    int len = this->config.Size();
    
    for (int i = 0; i < len; i++)
    {
        if (this->config[i].HasMember(CHAPTER) &&
            (!this->config[i][CHAPTER].HasMember(chapChar) || !this->config[i][CHAPTER][chapChar].GetBool())) continue;
        fileName = this->config[i][NAME].GetString();
        path = FileUtils::getInstance()->fullPathForFilename("event/" + fileName + ES_EXTENSION);
        if (path == "") cout << "Can not load " << fileName << " as a common event script!" << endl;;
        EventScript* es {EventScript::create(fileName)};
        CC_SAFE_RETAIN(es);
        this->eventScripts[fileName] = es;
    }
    return true;
}

// イベントスクリプトの解放
void CommonEventScripts::releaseEventScripts()
{
    this->eventScripts.clear();
}

// イベントスクリプトを取得
EventScript* CommonEventScripts::getScript(const string &fileName)
{
    if (this->eventScripts.count(fileName) == 0) {
        LastSupper::AssertUtils::fatalAssert("CommonEventScriptError\n" + fileName + " is missing.");
        return nullptr;
    }
    return this->eventScripts[fileName];
}
