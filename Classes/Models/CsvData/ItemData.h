//
//  ItemData.h
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/15.
//
//

#ifndef ItemData_h
#define ItemData_h

#include "define.h"
#include "Utils/CsvUtils.h"

class ItemData : public Ref
{
public:
    enum struct Keys
    {
        ID,
        NAME,
        DISCRIPTION,
    };
    
// Instance valiables
private:
    CsvUtils::CsvMap data;

// Class methods
public:
    CREATE_FUNC(ItemData)
private:
    ItemData(){FUNCLOG};
    ~ItemData(){FUNCLOG};
    
// Instance methods
private:
    bool init();
public:
    string getItemName(const int item_id);
    string getItemDiscription(const int item_id);
};

#endif /* ItemData_hpp */
