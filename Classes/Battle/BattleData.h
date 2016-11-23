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
    
// 初期化
private:
    BattleData();
    ~BattleData();
    bool init();
};

#endif /* BattleData_h */
