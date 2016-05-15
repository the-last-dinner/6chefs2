//
//  CommonEventScripts.h
//  6chefs2
//
//  Created by Ryoya Ino on 2016/05/15.
//
//

#ifndef CommonEventScripts_h
#define CommonEventScripts_h

#include "define.h"

class GameEvent;
class EventScript;

class CommonEventScripts : public Ref
{
public:
    typedef map<string, EventScript*> scripts;
    static const char* NAME;
    static const char* CHAPTER;
    static const char* ALL;
    // クラスメソッド
public:
    CREATE_FUNC(CommonEventScripts)
private:
    CommonEventScripts(){FUNCLOG};
    ~CommonEventScripts(){FUNCLOG};
    
    // インスタンス変数
private:
    scripts eventScripts {};
    rapidjson::Document config {};
public:
    
    // インスタンスメソッド
private:
    bool init();
    bool getEventScriptsConfig();
    
public:
    EventScript* getScript(const string& fileName);
    bool loadEventScripts(const int chapter);
    void releaseEventScripts();
};

#endif /* CommonEventScript_h */
