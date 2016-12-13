//
//  BattleData.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#ifndef BattleData_h
#define BattleData_h

#include "define.h"

class GameEvent;

class BattleData : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC(BattleData);
    
// インスタンス変数
private:
    vector<int> _targetObjectIds {};
    GameEvent* _successCallbackEvent { nullptr };
    GameEvent* _failureCallbackEvent { nullptr };
    
// インスタンスメソッド
private:
    BattleData();
    ~BattleData();
    bool init();
    
public:
    void setTargetObjectIds(const vector<int>& ids);
    vector<int> getTargetObjectIds() const;
    
    void setSuccessCallbackEvent(GameEvent* event);
    GameEvent* getSuccessCallbackEvent() const;
    
    void setFailureCallbackEvent(GameEvent* event);
    GameEvent* getFailureCallbackEvent() const;
};

#endif /* BattleData_h */
