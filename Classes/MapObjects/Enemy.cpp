//
//  Enemy.cpp
//  LastSupper
//
//  Created by Kohei on 2015/11/18.
//
//

#include "MapObjects/Enemy.h"

#include "CocosStudio/CSNode.h"

#include "MapObjects/DetectionBox/AttackDetector.h"
#include "MapObjects/DetectionBox/CollisionDetector.h"
#include "MapObjects/MapObjectList.h"
#include "MapOBjects/Party.h"

#include "MapObjects/MovePatterns/MovePattern.h"
#include "MapObjects/MovePatterns/MovePatternFactory.h"

// コンストラクタ
Enemy::Enemy() { FUNCLOG }

// デストラクタ
Enemy::~Enemy()
{
    FUNCLOG
    
    CC_SAFE_RELEASE_NULL(_attackBox);
}

// 初期化
bool Enemy::init(const EnemyData& data)
{
    if (!Character::init(data.chara_data)) return false;
    
    _data = data;
    
    // 動きのアルゴリズムを生成
    _movePattern = MovePatternFactory::create()->createMovePattern(data.move_pattern, this);
    CC_SAFE_RETAIN(_movePattern);
    
    // 速さの倍率を設定
    if (_movePattern) _movePattern->setSpeedRatio(data.speed_ratio);
    
    // 最初に通る経路オブジェクトIDを設定
    if (_movePattern) _movePattern->setStartPathId(data.start_path_id);
    
    // 攻撃判定を生成
    AttackBox* attackBox { AttackBox::create(this, _csNode->getCSChild(CS_ATTACK_NODE_NAME), nullptr) };
    CC_SAFE_RETAIN(attackBox);
    _attackBox = attackBox;
    
    return true;
}

// 指定のMapObjectに対して当たり判定があるか
bool Enemy::isHit(const MapObject* other) const
{
    Character* mainCharacter { _objectList->getParty()->getMainCharacter() };
    
    if (other == mainCharacter) return false;
    
    return true;
}

// 敵IDを取得
int Enemy::getEnemyId() const
{
    return _data.enemy_id;
}

// データを取得
EnemyData Enemy::getEnemyData() const
{
    EnemyData data { _data };
    
    // 位置情報を更新
    data.chara_data.location = this->getCharacterData().location;
    
    return data;
}

// マップ移動可能か
bool Enemy::canGoToNextMap() const
{
    if (!_movePattern) return false;
    
    return _movePattern->canGoToNextMap();
}

// 現在座標から、主人公までかかる時間を計算
float Enemy::calcSummonDelay() const
{
    if (!_movePattern) return 0.0f;
    
    return _movePattern->calcSummonDelay();
}

#pragma mark -
#pragma mark Interface

// マップに配置された時
void Enemy::onEnterMap()
{
    Character::onEnterMap();
    _objectList->getAttackDetector()->addAttackBox(_attackBox);
}

// マップから削除された時
void Enemy::onExitMap()
{
    Character::onExitMap();
    _objectList->getAttackDetector()->removeAttackBox(_attackBox);
}

// バトル開始時
void Enemy::onBattleStart()
{
    Character::onBattleStart();
    _objectList->getAttackDetector()->removeAttackBox(_attackBox);
}

// バトル終了時
void Enemy::onBattleFinished()
{
    Character::onBattleFinished();
    _objectList->getAttackDetector()->addAttackBox(_attackBox);
}

// イベント終了時
void Enemy::onEventFinished()
{
    Character::onEventFinished();
    _movePattern->start();
}
