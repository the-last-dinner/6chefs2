//
//  MapData.h
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/16.
//
//

#ifndef MapData_h
#define MapData_h

#include "define.h"
#include "Utils/CsvUtils.h"

class MapData : public Ref
{
public:
    enum struct Keys
    {
        ID,
        NAME,
        FILE_NAME
    };
    
    // Instance valiables
private:
    CsvUtils::CsvMap data;
    
    // Class methods
public:
    CREATE_FUNC(MapData)
private:
    MapData(){FUNCLOG};
    ~MapData(){FUNCLOG};
    
    // Instance methods
private:
    bool init();
public:
    string getName(const int map_id);
    string getFileName(const int map_id);
    vector<string> getFileNameAll();
};

#endif /* MapData_h */
