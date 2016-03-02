//
//  StringUtils.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/10/12.
//
//

#ifndef __LastSupper__StringUtils__
#define __LastSupper__StringUtils__

#include "define.h"

namespace LastSupper
{
    namespace StringUtils
    {
        string strReplace(const string& pattern, const string& replacement, string target);
        string getSprintf(const string& format, const string& str);
        string splitLineBreak(const string& target, const int line_limit);
        string getRandomString(const int length = 50);
        string getTimeForDisplay(const int sec);
        void encryptXor(string& str);
    }
}

#endif /* defined(__LastSupper__StringUtils__) */
