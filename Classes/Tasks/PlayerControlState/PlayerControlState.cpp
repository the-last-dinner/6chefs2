//
//  PlayerControlState.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/20.
//
//

#include "Tasks/PlayerControlState/PlayerControlState.h"

// コンストラクタ
PlayerControlState::PlayerControlState() {}

// デストラクタ
PlayerControlState::~PlayerControlState() {}

// 初期化
bool PlayerControlState::init(PlayerControlTask* task)
{
    if (!task) return false;
    _task = task;
    
    return true;
}
