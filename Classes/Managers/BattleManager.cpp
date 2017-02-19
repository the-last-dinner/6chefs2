//
//  BattleManager.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/11.
//
//

#include "Managers/BattleManager.h"

#include "Battle/Battle.h"

// 唯一のインスタンスを初期化
static BattleManager* _instance = nullptr;

// インスタンスの生成&取得
BattleManager* BattleManager::getInstance()
{
    if(!_instance) _instance = new BattleManager();
    return _instance;
}

// インスタンスの消去
void BattleManager::destroy()
{
    delete _instance;
}

// デストラクタ
BattleManager::~BattleManager()
{
    FUNCLOG
    
    CC_SAFE_RELEASE_NULL(_battle);
}

// コンストラクタ
BattleManager::BattleManager() { FUNCLOG }

void BattleManager::setBattleInstance(Battle* battle)
{
    CC_SAFE_RELEASE_NULL(_battle);
    CC_SAFE_RETAIN(battle);
    battle->_finishCallback = CC_CALLBACK_1(BattleManager::onBattleFinished, this);
    
    _battle = battle;
}


void BattleManager::onBattleFinished(Battle* finishedBattle)
{
    CC_SAFE_RELEASE_NULL(_battle);
}

bool BattleManager::isInBattle() const
{
    return _battle;
}
