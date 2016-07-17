//
//  AssertUtils.h
//  6chefs2
//
//  Created by S.inami on 2016/07/13.
//
//

#ifndef AssertUtils_h
#define AssertUtils_h

#include "define.h"

class AssertScene;

namespace LastSupper
{
    namespace AssertUtils
    {
        void infoAssert(const string& message);
        void warningAssert(const string& message);
        void fatalAssert(const string& message);
    }
}

#endif /* AssertUtils_h */
