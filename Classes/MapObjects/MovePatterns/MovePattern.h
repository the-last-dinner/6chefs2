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
    Character* _chara { nullptr };
    float _speedRatio { 1.0f };
    bool _started { false };
    bool _paused { false };
    int _startPathId { -1 };

// インスタンスメソッド
public:
    virtual void start();
    virtual void pause();
    virtual void resume();
    bool isPaused() const;
    bool hasSterted() const;
    virtual void onPartyMoved() {};
    virtual bool canGoToNextMap() const { return false; };
    virtual float calcSummonDelay() const { return 0.0f; };
    virtual void setSpeedRatio(float ratio);
    void setStartPathId(const int pathId);
protected:
    MovePattern();
    ~MovePattern();
    virtual bool init(Character* chara);
    Character* getMainCharacter() const;
    MapObjectList* getMapObjectList() const;

// インターフェース
public:
    virtual void update(float delta);
};

#endif /* defined(__LastSupper__MovePattern__) */
