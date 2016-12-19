//
//  EventScript.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/03/15.
//
//

#ifndef __LastSupper__EventScriptManager__
#define __LastSupper__EventScriptManager__

#include "define.h"

class EventScript : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EventScript, const string&)
    
private:
    EventScript();   // コンストラクタ
    ~EventScript();  // デストラクタ
    
//インスタンス変数
private:
    rapidjson::Document _json;
    string _fileName;
    
// インスタンスメソッド
public:
    bool init(const string& jsonFileName);              // 初期化
    vector<string> getPreLoadList(const string& type);         // 音楽などのリソースのプリロード
    rapidjson::Value getScriptJson(const int eventId);           // 該当IDスクリプトの取得
    rapidjson::Value getScriptJson(const string& eventId);
    rapidjson::Document getDocument();
};
#endif /* defined(__LastSupper__EventScript__) */
