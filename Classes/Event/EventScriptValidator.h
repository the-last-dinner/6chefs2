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
    CREATE_FUNC(EventScriptValidator)
private:
    EventScriptValidator() { FUNCLOG };
    ~EventScriptValidator() { FUNCLOG };
    
// instance valiables
private:
    rapidjson::Document validateConfig {};
    
// instance methods
public:
    bool init();
    bool validate(const rapidjson::Value& json);
};

#endif /* EventScriptValidator_hpp */
