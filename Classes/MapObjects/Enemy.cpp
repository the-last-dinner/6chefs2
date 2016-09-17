//
//  Enemy.cpp
//  LastSupper
//
//  Created by Kohei on 2015/11/18.
//
//

#include "MapObjects/Enemy.h"

#include "MapObjects/DetectionBox/CollisionDetector.h"
#include "MapObjects/MapObjectList.h"
#include "MapOBjects/Party.h"

#include "MapObjects/MovePatterns/MovePattern.h"
#include "MapObjects/MovePatterns/MovePatternFactory.h"

// コンストラクタ
Enemy::Enemy() { FUNCLOG }

// デストラクタ
Enemy::~Enemy() { FUNCLOG }

// 初期化
bool Enemy::init(const EnemyData& data)
{
    if(!Character::init(data.chara_data)) return false;
    
    this->data = data;
    
    // 動きのアルゴリズムを生成
    MovePatternFactory* factory { MovePatternFactory::create() };
    CC_SAFE_RETAIN(factory);
    _movePattern = factory->createMovePattern(data.move_pattern, this);
    CC_SAFE_RETAIN(_movePattern);
    CC_SAFE_RELEASE(factory);
    
    // 速さの倍率を設定
    if(_movePattern) _movePattern->setSpeedRatio(data.speed_ratio);
    
    // 最初に通る経路オブジェクトIDを設定
    if(_movePattern) _movePattern->setStartPathId(data.start_path_id);
    
    return true;
}

// 指定のMapObjectに対して当たり判定があるか
bool Enemy::isHit(const MapObject* other) const
{
    Character* mainCharacter { _objectList->getParty()->getMainCharacter() };
    
    if(other == mainCharacter) return false;
    
    return true;
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
    if(!_movePattern) return false;
    
    return _movePattern->canGoToNextMap();
}

// 現在座標から、主人公までかかる時間を計算
float Enemy::calcSummonDelay() const
{
    if(!_movePattern) return 0.0f;
    
    return _movePattern->calcSummonDelay();
}
