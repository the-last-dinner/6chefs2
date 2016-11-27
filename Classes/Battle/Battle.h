//
//  Battle.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#ifndef Battle_h
#define Battle_h

#include "define.h"

class BattleData;

class Battle : public Node
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Battle, BattleData*);
    
// インスタンス変数
private:
    BattleData* _data { nullptr };
    
// インスタンスメソッド
private:
    Battle();
    ~Battle();
    bool init(BattleData* data);
};

#endif /* Battle_hpp */
