//
//  MobRandom.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/19.
//
//

#ifndef MobRandom_h
#define MobRandom_h

#include "MapObjects/MovePatterns/MovePattern.h"

class MobRandom : public MovePattern
{
// 定数
private:
    static const float MIN_WAIT_DURATION;
    static const float MAX_WAIT_DURATION;
    static const int DISTANCE;
    static const float SPEED_RATIO;
    static const int SCHEDULE_ACTION_TAG;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(MobRandom, Character*)
    
// インスタンス変数
private:
    Point _homePosition { Point::ZERO };
    
// インスタンスメソッド
private:
    MobRandom();
    ~MobRandom();
    virtual bool init(Character* chara) override;
    virtual void start() override;
    virtual void pause() override;
    void move();
    void scheduleMove();
};

#endif /* MobRandom_h */
