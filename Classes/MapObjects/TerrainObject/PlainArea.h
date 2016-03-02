//
//  PlainArea.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#ifndef PlainArea_h
#define PlainArea_h

#include "MapObjects/TerrainObject/TerrainObject.h"

class PlainArea : public TerrainObject
{
// クラスメソッド
public:
    CREATE_FUNC(PlainArea)
    
// インスタンスメソッド
private:
    PlainArea();
    ~PlainArea();
    bool init();
public:
    virtual void onWillMove(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio);
    virtual float getStaminaConsumptionRate() const { return 1.f; };
    virtual bool canDash() const { return true; };
    virtual bool consumeStaminaWalking() const { return false; };
};

#endif /* PlainArea_h */
