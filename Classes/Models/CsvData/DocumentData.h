//
//  DocumentData.h
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/15.
//
//

#ifndef DocumentData_h
#define DocumentData_h

#include "define.h"
#include "Utils/CsvUtils.h"

class DocumentData : public Ref
{
public:
    enum struct Keys
    {
        ID,
        NAME,
        IMAGE,
        PROFILE0,
        PROFILE1,
        PROFILE2,
    };
    // Instance valiables
private:
    CsvUtils::CsvMap data;
    
    // Class methods
public:
    CREATE_FUNC(DocumentData)
private:
    DocumentData(){FUNCLOG};
    ~DocumentData(){FUNCLOG};
    
    // Instance methods
private:
    bool init();
    
public:
    string getName(const int docId);
    string getImageFileName(const int docId);
    string getDiscription(const int docId, const int level);
    vector<int> getDocumentDataIds();
    int getDocumentCount();
};

#endif /* DocumentData_h */
