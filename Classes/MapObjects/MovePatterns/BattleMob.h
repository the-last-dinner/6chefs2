//
//  BattleMob.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/11.
//
//

#ifndef BattleMob_h
#define BattleMob_h

#include "MapObjects/MovePatterns/MovePattern.h"

class Chaser;

class BattleMob : public MovePattern
{
// 定数
private:
    static const string ATTACK_NAME;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(BattleMob, Character*)
    
// インスタンス変数
private:
    Chaser* _subPattern { nullptr };
    
// インスタンスメソッド
private:
    BattleMob();
    ~BattleMob();
    virtual bool init(Character* character) override;
    
// インターフェース
public:
    virtual void start() override;
    virtual void pause() override;
    virtual void resume() override;
    virtual void onPartyMoved() override;
    virtual bool canGoToNextMap() const override;
    virtual void update(float delta) override;
};

#endif /* BattleMob_h */
