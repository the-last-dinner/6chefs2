//
//  BothFollow.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#ifndef __LastSupper__BothFollow__
#define __LastSupper__BothFollow__

#include "Tasks/FollowType/FollowType.h"

class BothFollow : public FollowType
{
// クラスメソッド
public:
    CREATE_FUNC(BothFollow)
    
// インスタンスメソッド
private:
    BothFollow();
    ~BothFollow();
    virtual bool init() override;
    Point getPosition(const Size& mapSize, const Point& targetPosition) override;
};

#endif /* defined(__LastSupper__BothFollow__) */
