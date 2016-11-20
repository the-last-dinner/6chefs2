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

class PlayerControlState : public Ref
{
// インスタンスメソッド
protected:
    PlayerControlState();
    ~PlayerControlState();
    virtual bool init();
    
// インターフェース
public:
    virtual void onEnterKeyPressed(Party* party) = 0;
};

#endif /* PlayerControlState_h */
