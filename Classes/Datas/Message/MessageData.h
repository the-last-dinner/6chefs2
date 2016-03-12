//
//  MessageData.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/12.
//
//

#ifndef __LastSupper__MessageData__
#define __LastSupper__MessageData__

#include "Common.h"

class MessageData : public Ref
{
// インスタンスメソッド
public:
    virtual bool hasNextPage(){return true;};
    virtual const string getMessage() = 0;
protected:
    MessageData();
    virtual ~MessageData();
};

#endif /* defined(__LastSupper__MessageData__) */
