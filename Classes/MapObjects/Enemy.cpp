//
//  Enemy.cpp
//  LastSupper
//
//  Created by Kohei on 2015/11/18.
//
//

#include "MapObjects/Enemy.h"

#include "MapObjects/MapObjectList.h"
#include "MapOBjects/Party.h"

#include "MapObjects/MovePatterns/MovePattern.h"
#include "MapObjects/MovePatterns/MovePatternFactory.h"

// コンストラクタ
Enemy::Enemy() {FUNCLOG};

// デストラクタ
Enemy::~Enemy() {FUNCLOG};

// 初期化
bool Enemy::init(const EnemyData& data)
{
    if(!Character::init(data.chara_data)) return false;
    
    this->data = data;
    
    // 動きのアルゴリズムを生成
    MovePatternFactory* factory { MovePatternFactory::create() };
    CC_SAFE_RETAIN(factory);
    this->movePattern = factory->createMovePattern(data.move_pattern, this);
    CC_SAFE_RETAIN(this->movePattern);
    CC_SAFE_RELEASE(factory);
    
    // 速さの倍率を設定
    if(this->movePattern) this->movePattern->setSpeedRatio(data.speed_ratio);
    
    // 最初に通る経路オブジェクトIDを設定
    if(this->movePattern) this->movePattern->setStartPathId(data.start_path_id);
    
    return true;
}

// 主人公と自身に対して当たり判定を適用しないようにする
const bool Enemy::isHit(const vector<Direction>& directions) const
{
    if(!this->objectList) return false;
    
    // 自身以外の当たり判定を持つオブジェクトが、指定方向にあればtrueを返す
    for(MapObject* obj : this->objectList->getMapObjects(this->getCollisionRect(directions)))
    {
        if(obj == this || obj == this->objectList->getParty()->getMainCharacter()) continue;
        
        if(obj->isHit()) return true;
    }
    
    return false;
}

// 敵IDを取得
int Enemy::getEnemyId() const
{
    return this->data.enemy_id;
}

// データを取得
EnemyData Enemy::getEnemyData() const
{
    EnemyData data {this->data};
    
    // 位置情報を更新
    data.chara_data.location = this->getCharacterData().location;
    
    return data;
}

// マップ移動可能か
bool Enemy::canGoToNextMap() const
{
    if(!this->movePattern) return false;
    
    return this->movePattern->canGoToNextMap();
}

// 現在座標から、主人公までかかる時間を計算
float Enemy::calcSummonDelay() const
{
    if(!this->movePattern) return 0.0f;
    
    return this->movePattern->calcSummonDelay();
}
