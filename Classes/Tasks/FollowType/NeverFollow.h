//
//  NeverFollow.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#ifndef __LastSupper__NeverFollow__
#define __LastSupper__NeverFollow__

#include "Tasks/FollowType/FollowType.h"

class NeverFollow : public FollowType
{
// クラスメソッド
public:
    CREATE_FUNC(NeverFollow);
    
// インスタンスメソッド
private:
    NeverFollow();
    ~NeverFollow();
    virtual bool init() override;
    virtual Point getPosition(const Size& mapSize, const Point& targetPosition) override;
};

#endif /* defined(__LastSupper__NeverFollow__) */
