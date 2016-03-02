//
//  FollowType.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#ifndef __LastSupper__Follow__
#define __LastSupper__Follow__

#include "Common.h"

class FollowType : public Ref
{
// インスタンス変数
private:
    
// インスタンスメソッド
public:
    FollowType();
    ~FollowType();
    virtual Point getPosition(const Size& mapSize, const Point& targetPosition) = 0;
protected:
    virtual bool init();
};

#endif /* defined(__LastSupper__Follow__) */
