//
//  TerrainObject.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#ifndef TerrainObject_h
#define TerrainObject_h

#include "MapObjects/MapObject.h"

class Character;

class TerrainObject : public MapObject
{
// インスタンスメソッド
protected:
    TerrainObject();
    ~TerrainObject();
    int getStampingState(Character* target) const;
    void setStampingState(Character* target, int state);
    string getPrefix(Character* target) const;
public:
    virtual void onWillMove(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio) = 0;
    virtual void onWillStamp(Character* target, const Direction& direction, const float ratio);
    virtual float getStaminaConsumptionRate() const = 0;
    virtual bool canDash() const = 0;
    virtual bool consumeStaminaWalking() const = 0;
    virtual string getDotPrefix() const { return "";};
};

#endif /* TerrainObject_h */
