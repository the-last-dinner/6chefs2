//
//  Enemy.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/18.
//
//

#ifndef __LastSupper__Enemy__
#define __LastSupper__Enemy__

#include "MapObjects/Character.h"

#include "Datas/Mapobject/EnemyData.h"

class AttackBox;

class Enemy : public Character
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Enemy, const EnemyData&)

// インスタンス変数
private:
    EnemyData _data {};
    AttackBox* _attackBox { nullptr };
    
// インスタンスメソッド
private:
    Enemy();
    ~Enemy();
    bool init(const EnemyData& data);
    virtual bool isHit(const MapObject* other) const override;
public:
    int getEnemyId() const;
    EnemyData getEnemyData() const;
    bool canGoToNextMap() const;
    float calcSummonDelay() const;

// Battle
public:
    bool canAttack(MapObject* target) const override;
    
// インターフェース
public:
    virtual void onEnterMap() override;
    virtual void onExitMap() override;
    virtual void onBattleStart(Battle* battle) override;
    virtual void onBattleFinished() override;
    virtual void onEventFinished() override;
};

#endif /* defined(__LastSupper__Enemy__) */
