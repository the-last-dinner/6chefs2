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

class Enemy : public Character
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Enemy, const EnemyData&)

// インスタンス変数
private:
    EnemyData data {};
    
// インスタンスメソッド
private:
    Enemy();
    ~Enemy();
    bool init(const EnemyData& data);
    virtual const bool isHit(const vector<Direction>& directions) const;
public:
    int getEnemyId() const;
    EnemyData getEnemyData() const;
    bool canGoToNextMap() const;
    float calcSummonDelay() const;
};

#endif /* defined(__LastSupper__Enemy__) */
