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
    CREATE_FUNC(SearchState);
    
// インスタンスメソッド
private:
    SearchState();
    ~SearchState();
    virtual bool init() override;

// インターフェース
protected:
    virtual void onEnterKeyPressed(Party* party) override;
    
};

#endif /* SearchState_h */
