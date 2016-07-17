//
//  VerticalFollow.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#ifndef __LastSupper__VerticalFollow__
#define __LastSupper__VerticalFollow__

#include "Tasks/FollowType/FollowType.h"

class VerticalFollow : public FollowType
{
// クラスメソッド
public:
    CREATE_FUNC(VerticalFollow)
    
// インスタンスメソッド
private:
    VerticalFollow();
    ~VerticalFollow();
    virtual bool init() override;
    Point getPosition(const Size& mapSize, const Point& targetPosition) override;
};

#endif /* defined(__LastSupper__VerticalFollow__) */
