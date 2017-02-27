//
//  BattleManager.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/11.
//
//

#ifndef BattleManager_h
#define BattleManager_h

#include "define.h"

class Battle;

class BattleManager
{
// singleton用関数
public:
    static BattleManager* getInstance();
    static void destroy();
private:
    BattleManager();  // コンストラクタ
    ~BattleManager(); // デストラクタ
    BattleManager(const BattleManager& other){}; // コピーコンストラクタ
    BattleManager& operator = (const BattleManager& other); // 代入演算子

// インスタンス変数
private:
    Battle* _battle { nullptr };
    
// インスタンスメソッド
public:
    void setBattleInstance(Battle* battle);
    void onBattleFinished(Battle* finishedBattle);
    bool isInBattle() const;
};

#endif /* BattleManager_h */
