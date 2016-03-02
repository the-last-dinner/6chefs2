//
//  RandomMove.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/16.
//
//

#ifndef __LastSupper__RandomMove__
#define __LastSupper__RandomMove__

#include "MapObjects/MovePatterns/MovePattern.h"

class RandomMove : public MovePattern
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(RandomMove, Character*);

// インスタンス変数
private:
    float second {0.f};
    
// インスタンスメソッド
private:
    RandomMove();
    ~RandomMove();
    bool init(Character* chara);
    virtual void start() override;
    virtual void move();
};

#endif /* defined(__LastSupper__RandomMove__) */
