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

class BattleData : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC(BattleData);
    
// インスタンス変数
private:
    vector<int> _targetObjectIds {};
    
// インスタンスメソッド
private:
    BattleData();
    ~BattleData();
    bool init();
    
public:
    void setTargetObjectIds(const vector<int>& ids);
    vector<int> getTargetObjectIds() const;
};

#endif /* BattleData_h */
