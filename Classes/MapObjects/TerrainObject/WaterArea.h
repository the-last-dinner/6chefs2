//
//  WaterArea.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#ifndef Water_h
#define Water_h

#include "MapObjects/TerrainObject/TerrainObject.h"

class WaterArea : public TerrainObject
{
// 定数
private:
    static const float SPEED_RATIO;
    
// クラスメソッド
public:
    CREATE_FUNC(WaterArea)

// インスタンスメソッド
private:
    WaterArea();
    ~WaterArea();
    bool init();
public:
    virtual void onWillMove(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio);
    virtual void onWillStamp(Character* target, const Direction& direction, const float ratio) override;
    virtual float getStaminaConsumptionRate() const { return 2.0f; };
    virtual bool canDash() const { return false; };
    virtual bool consumeStaminaWalking() const { return true; };
    virtual string getDotPrefix() const override { return "swim_"; };
};

#endif /* Water_h */
