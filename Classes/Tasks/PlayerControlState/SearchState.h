//
//  SearchState.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/20.
//
//

#ifndef SearchState_h
#define SearchState_h

#include "Tasks/PlayerControlState/PlayerControlState.h"

class SearchState : public PlayerControlState
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(SearchState, PlayerControlTask*);
    
// インスタンスメソッド
private:
    SearchState();
    ~SearchState();
    virtual bool init(PlayerControlTask* task) override;

// インターフェース
protected:
    virtual void turn(Party* party, const Direction& direction, bool isDashKeyPressed) override;
    virtual void onEnterKeyPressed(Party* party) override;
    virtual void move(Party* party, const vector<Direction>& directions, bool isDashKeyPressed) override;
};

#endif /* SearchState_h */
