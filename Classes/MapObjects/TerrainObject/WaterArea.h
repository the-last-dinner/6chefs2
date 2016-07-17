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
    bool init() override;
public:
    virtual void onWillMove(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio) override;
    virtual void onWillStamp(Character* target, const Direction& direction, const float ratio) override;
    virtual float getStaminaConsumptionRate() const override { return 2.0f; };
    virtual bool canDash() const override { return false; };
    virtual bool consumeStaminaWalking() const override { return true; };
    virtual string getDotPrefix() const override { return "swim_"; };
};

#endif /* Water_h */
