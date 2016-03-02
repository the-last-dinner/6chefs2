//
//  Scouter.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#ifndef Scouter_h
#define Scouter_h

#include "MapObjects/MovePatterns/MovePattern.h"

class Sight;
class Chaser;
class PathFinder;
class PathObject;

class Scouter : public MovePattern
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Scouter, Character*)

// インスタンス変数
private:
    Sight* sight { nullptr };
    Chaser* subPattern { nullptr };
    PathFinder* finder { nullptr };
    deque<Direction> path {};
    
// インスタンスメソッド
private:
    Scouter();
    ~Scouter();
    virtual bool init(Character* character) override;
    deque<Direction> getPath(PathObject* pathObject);
    void shiftToSubPattern();
    void onTargetCameIntoSight();
public:
    virtual void start() override;
    virtual void setPaused(bool paused) override;
    virtual void onPartyMoved() override;
    virtual bool canGoToNextMap() const override;
    virtual float calcSummonDelay() const override;
    void move(const int pathObjId);
    void update(float _);
};

#endif /* Scouter_h */
