//
//  SlipFloor.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#ifndef SlipFloor_h
#define SlipFloor_h

#include "MapObjects/TerrainObject/TerrainObject.h"

class SlipFloor : public TerrainObject
{
// クラスメソッド
public:
    CREATE_FUNC(SlipFloor)
    
// インスタンスメソッド
private:
    SlipFloor();
    ~SlipFloor();
    bool init();
    
public:
    virtual void onWillMove(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio);
    virtual float getStaminaConsumptionRate() const { return 1.0f; };
    virtual bool canDash() const { return false; };
    virtual bool consumeStaminaWalking() const { return false; };
};

#endif /* SlipFloor_h */
