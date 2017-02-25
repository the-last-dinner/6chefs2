//
//  MasterConfigData.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/28.
//
//

#ifndef MasterConfigData_h
#define MasterConfigData_h

#include "define.h"

class MasterConfigData : public Ref
{
    // Class methods
public:
    CREATE_FUNC(MasterConfigData)
private:
    MasterConfigData(){FUNCLOG};
    ~MasterConfigData(){FUNCLOG};
    
    // constants
public:
    static const char* VERSION;
    static const char* COPYRIGHT;
    static const char* DISPLAY;
    static const char* TWO_ICON;
    static const char* FRIENDSHIP;
    static const char* SPECIAL_ROOM;
    static const char* OPENING_SCENE;
    static const char* OPENING_VIDEO_FILE;
    static const char* OPENING_BGM_FILE;
    
    // instance valiables
private:
    rapidjson::Document masterConfig {nullptr};
    string filePath {};
    
    // instance methods
public:
    bool init();
    bool isDisplay(const char* targetProperty);
    string getString(const char* targetProperty);
};

#endif /* MasterConfigData_hpp */
