//
//  CharacterMasterData.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/16.
//
//

#ifndef CharacterMasterData_h
#define CharacterMasterData_h

#include "define.h"
#include "Utils/CsvUtils.h"

class CharacterMasterData : public Ref
{
public:
    enum struct Keys
    {
        ID,
        NAME,
        RUBY,
        STREET_NAME,
        FILE_NAME_INI,
        FACE_DIFF,
        PROFILE_FLAG,
        PROFILE0,
        PROFILE1,
        PROFILE2
    };
    
    // Instance valiables
private:
    CsvUtils::CsvMap data;
    
    // Class methods
public:
    CREATE_FUNC(CharacterMasterData)
private:
    CharacterMasterData(){FUNCLOG};
    ~CharacterMasterData(){FUNCLOG};
    
    // Instance methods
private:
    bool init();
public:
    string getName(const int chara_id);
    string getNameWithRuby(const int chara_id);
    string getRuby(const int chara_id);
    string getRubyWithBracket(const int chara_id);
    string getStreetName(const int chara_id);
    string getFileName(const int chara_id);
    int getFaceDiff(const int chara_id);
    string getDiscription(const int chara_id, const int level);
    bool isDisplay(const int chara_id);
    vector<int> getDisplayCharacters();
};

#endif /* CharacterMasterData_hpp */
