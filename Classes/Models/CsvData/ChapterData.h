//
//  ChapterData.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/16.
//
//

#ifndef ChapterData_h
#define ChapterData_h

#include "define.h"
#include "Utils/CsvUtils.h"

class ChapterData : public Ref
{
public:
    enum struct Keys
    {
        ID,
        NAME,
        TITLE
    };
    
    // Instance valiables
private:
    CsvUtils::CsvMap data;
    
    // Class methods
public:
    CREATE_FUNC(ChapterData)
private:
    ChapterData(){FUNCLOG};
    ~ChapterData(){FUNCLOG};
    
    // Instance methods
private:
    bool init();
public:
    string getName(const int chapter_id);
    string getTitle(const int chapter_id);
};

#endif /* ChapterData_hpp */
