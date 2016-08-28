//
//  TrophyData.h
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/16.
//
//

#ifndef TrophyData_h
#define TrophyData_h

#include "define.h"
#include "Utils/CsvUtils.h"

class TrophyData : public Ref
{
public:
    enum struct Keys
    {
        ID,
        NAME,
        CONDITION,
        COMMENT
    };
    
    // Instance valiables
private:
    CsvUtils::CsvMap data;
    
    // Class methods
public:
    CREATE_FUNC(TrophyData)
private:
    TrophyData(){FUNCLOG};
    ~TrophyData(){FUNCLOG};
    
    // Instance methods
private:
    bool init();
public:
    string getName(const int trophy_id);
    string getCondition(const int trophy_id);
    string getComment(const int trophy_id);
    vector<int> getIdAll();
};

#endif /* TrophyData_h */
