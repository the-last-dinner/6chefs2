//
//  HorizontalFollow.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#ifndef __LastSupper__HorizontalFollow__
#define __LastSupper__HorizontalFollow__

#include "Tasks/FollowType/FollowType.h"

class HorizontalFollow : public FollowType
{
// クラスメソッド
public:
    CREATE_FUNC(HorizontalFollow)

// インスタンスメソッド
private:
    HorizontalFollow();
    ~HorizontalFollow();
    virtual bool init() override;
    virtual Point getPosition(const Size& mapSize, const Point& targetPosition) override;
};

#endif /* defined(__LastSupper__HorizontalFollow__) */
