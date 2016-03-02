//
//  MovePattern.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/16.
//
//

#ifndef __LastSupper__MovePattern__
#define __LastSupper__MovePattern__

#include "Common.h"

class Character;
class MapObjectList;

class MovePattern : public Ref
{
// インスタンス変数
protected:
    Character* chara { nullptr };
    float speedRatio { 1.0f };
    bool paused { true };
    int startPathId { -1 };

// インスタンスメソッド
public:
    virtual void start();
    virtual void setPaused(bool paused);
    bool isPaused() const;
    virtual void onPartyMoved() {};
    virtual bool canGoToNextMap() const { return false; };
    virtual float calcSummonDelay() const { return 0.0f; };
    void setSpeedRatio(float ratio);
    void setStartPathId(const int pathId);
protected:
    MovePattern();
    ~MovePattern();
    virtual bool init(Character* chara);
    Character* getMainCharacter() const;
    MapObjectList* getMapObjectList() const;
};

#endif /* defined(__LastSupper__MovePattern__) */
