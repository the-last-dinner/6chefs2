//
//  BattleBoss.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/11.
//
//

#ifndef BattleBoss_h
#define BattleBoss_h

#include "MapObjects/MovePatterns/MovePattern.h"

class Chaser;

class BattleBoss : public MovePattern
{
// 定数
private:
    static const string FORWARD_ATTACK_NAME;
    static const string SPIN_ATTACK_NAME;
    static const string FLASH_ATTACK_NAME;
    static const float RANDOM_MOVE_RATIO;
    static const float SPIN_ATTACK_RATIO;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(BattleBoss, Character*)
    
// インスタンス変数
private:
    Chaser* _subPattern { nullptr };
    
// インスタンスメソッド
private:
    BattleBoss();
    ~BattleBoss();
    virtual bool init(Character* character) override;

// コールバック
private:
    void onForwardAttackCommandFinished(Character* character);
    void onSpinAttackCommandFinished(Character* charater);
    void onFastMoveCommandFinished();
    
// インターフェース
public:
    virtual void start() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void onPartyMoved() override;
    virtual bool canGoToNextMap() const override;
    virtual void update(float delta) override;
};

#endif /* BattleBoss_hpp */
