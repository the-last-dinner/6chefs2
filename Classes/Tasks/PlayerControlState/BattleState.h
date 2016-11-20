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

class BattleState : public PlayerControlState
{
// クラスメソッド
public:
    CREATE_FUNC(BattleState);
    
// インスタンスメソッド
private:
    BattleState();
    ~BattleState();
    virtual bool init() override;
    
// インターフェース
public:
    virtual void onEnterKeyPressed(Party* party) override;
};

#endif /* BattleState_h */
