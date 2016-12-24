//
//  BattleState.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/20.
//
//

#ifndef BattleState_h
#define BattleState_h

#include "Tasks/PlayerControlState/PlayerControlState.h"

class Character;

class BattleState : public PlayerControlState
{
// 定数
private:
    static const float STEP_COOLDOWN_DURATION;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(BattleState, PlayerControlTask*);
    
// インスタンスメソッド
private:
    BattleState();
    ~BattleState();
    virtual bool init(PlayerControlTask* task) override;
    
// インターフェース
public:
    virtual void turn(Party* party, const Direction& direction, bool isDashKeyPressed) override;
    virtual void onEnterKeyPressed(Party* party) override;
    virtual void move(Party* party, const vector<Direction>& directions, bool isDashKeyPressed) override;
};

#endif /* BattleState_h */
