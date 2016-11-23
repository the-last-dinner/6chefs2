//
//  Battle.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#include "Battle/Battle.h"

// コンストラクタ
Battle::Battle() { FUNCLOG }

// デストラクタ
Battle::~Battle() { FUNCLOG }

// 初期化
bool Battle::init(BattleData* data)
{
    if (!Node::init()) return false;
    if (!data) return false;
    
    _data = data;
    
    return true;
}

