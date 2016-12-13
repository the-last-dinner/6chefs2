//
//  PlayerControlState.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/20.
//
//

#ifndef PlayerControlState_h
#define PlayerControlState_h

#include "define.h"

class Party;
class PlayerControlTask;

class PlayerControlState : public Ref
{
// インスタンスメソッド
protected:
    PlayerControlState();
    ~PlayerControlState();
    virtual bool init(PlayerControlTask* task);
    
// インスタンス変数
protected:
    PlayerControlTask* _task { nullptr };
    
// インターフェース
public:
    virtual void turn(Party* party, const Direction& direction, bool isDashKeyPressed) = 0;
    virtual void onEnterKeyPressed(Party* party) = 0;
    virtual void move(Party* party, const vector<Direction>& directions, bool isDashKeyPressed) = 0;
};

#endif /* PlayerControlState_h */
