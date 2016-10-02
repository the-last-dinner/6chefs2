//
//  AssertHelper.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/09/27.
//
//

#ifndef AssertHelper_h
#define AssertHelper_h

#include "define.h"

class AssertHelper : public Ref
{
public:
    CREATE_FUNC(AssertHelper);
private:
    AssertHelper() { FUNCLOG };
    ~AssertHelper() { FUNCLOG };

// member
private:
    vector<string> textStack {};
    
public:
    bool init();
    AssertHelper* pushTextLineKeyValue(const string& key, const string& value);
    AssertHelper* pushTextLine(const string& text);
    AssertHelper* popTextLine();
    AssertHelper* popTextLines(const int count);
    void warningAssert(const string& title = "");
    void fatalAssert(const string& title = "");
private:
    string generateTextFromStack(const string& title);
};

#endif /* AssertHelper_hpp */
