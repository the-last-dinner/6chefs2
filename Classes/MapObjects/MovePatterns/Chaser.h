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
private:
    static const int PATH_FINDING_THRESHOLD;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Chaser, Character*)

// インスタンス変数
private:
    CheapChaser* _subPattern { nullptr };

// インスタンスメソッド
public:
    virtual void start() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void onPartyMoved() override;
    virtual void setSpeedRatio(float speed) override;
    virtual void move();
    void moveProc();
private:
    Chaser();
    ~Chaser();
    virtual bool init(Character* character) override;
    void shiftFromSubPattern();
    void shiftToSubPattern();
    bool needsShiftToSubPattern() const;
    void cutPath(deque<Direction>& path);
    deque<Direction> getPath() const;
    void onStuck();
    virtual bool canGoToNextMap() const override;
    virtual float calcSummonDelay() const override;
};

#endif /* Chaser_h */
