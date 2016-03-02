//
//  Chaser.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#ifndef Chaser_h
#define Chaser_h

#include "MapObjects/MovePatterns/MovePattern.h"

class PathFinder;
class CheapChaser;

class Chaser : public MovePattern
{
// 定数
public:
    static const int PATH_FINDING_THRESHOLD;
    static const int SHIFT_PATTERN_THRESHOLD;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Chaser, Character*)

// インスタンス変数
private:
    PathFinder* pathFinder { nullptr };
    CheapChaser* subPattern { nullptr };
    int partyMoveCount { 0 };

// インスタンスメソッド
public:
    virtual void setPaused(bool paused) override;
    virtual void start() override;
    virtual void onPartyMoved() override;
    virtual void move();
private:
    Chaser();
    ~Chaser();
    virtual bool init(Character* character) override;
    void shiftFromSubPattern();
    void shiftToSubPattern();
    bool needsShiftToSubPattern(const deque<Direction>& path);
    void cutPath(deque<Direction>& path);
    deque<Direction> getPath() const;
    virtual bool canGoToNextMap() const override;
    virtual float calcSummonDelay() const override;
};

#endif /* Chaser_h */
