//
//  Character.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/06/15.
//
//

#include "MapObjects/Character.h"

#include "CocosStudio/CSNode.h"

#include "Datas/MapObject/CharacterData.h"
#include "Datas/BattleCharacterData.h"

#include "MapObjects/Command/HurtCommand.h"
#include "MapObjects/DetectionBox/AttackDetector.h"
#include "MapObjects/DetectionBox/CollisionDetector.h"
#include "MapObjects/DetectionBox/HitBox.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/MovePatterns/MovePattern.h"
#include "MapObjects/MovePatterns/MovePatternFactory.h"
#include "MapObjects/Status/HitPoint.h"
#include "MapObjects/Status/Sight.h"
#include "MapObjects/TerrainState/TerrainState.h"

#include "Managers/DungeonSceneManager.h"

// 定数
const string Character::CS_SPRITE_NODE_NAME { "sprite" };
const string Character::CS_COLLISION_NODE_NAME { "collision" };
const string Character::CS_HIT_NODE_NAME { "hit" };
const string Character::CS_ATTACK_NODE_NAME { "attack" };
const string Character::CS_BATTLE_ATTACK_NODE_NAME { "battle_attack" };

// コンストラクタ
Character::Character() { FUNCLOG }

// デストラクタ
Character::~Character()
{
    FUNCLOG
    CC_SAFE_RELEASE_NULL(_movePattern);
    CC_SAFE_RELEASE_NULL(_sight);
    CC_SAFE_RELEASE_NULL(_battleData);
}

// 初期化
bool Character::init(const CharacterData& data)
{
	if (!MapObject::init()) return false;
    
	// 生成時の情報をセット
    _charaId = data.chara_id;
    _location = data.location;
    this->setObjectId(data.obj_id);
    
    CSNode* csNode { CSNode::create(data.getCsbFilePath()) };
    if (!csNode) {
        csNode = CSNode::create("character/nadeshiko.csb"); // これ、サービスね
        //LastSupper::AssertUtils::fatalAssert("キャラクターのcsbファイルが存在しません\nFilePath : " + data.getCsbFilePath());
        //return false;
    }
    
    Node* spriteNode { csNode->getCSChild(CS_SPRITE_NODE_NAME) };
    
    if (!spriteNode) {
        LastSupper::AssertUtils::fatalAssert("キャラクターのcsbファイルに'sprite'オブジェクトが定義されていません");
        return false;
    }
    
    csNode->setPosition(-spriteNode->getPosition());
    this->addChild(csNode);
    _csNode = csNode;
    
    if (!_movePattern) {
        // 動きのアルゴリズムを生成
        _movePattern = MovePatternFactory::create()->createMovePattern(data.move_pattern, this);
        CC_SAFE_RETAIN(_movePattern);
    }

    // サイズ、衝突判定範囲をセット
    this->setContentSize(spriteNode->getContentSize());
    this->setCollision(CollisionBox::create(this, csNode->getCSChild(CS_COLLISION_NODE_NAME)));
    
    // くらい判定生成
    HitBox* hitBox { HitBox::create(this, _csNode->getCSChild(CS_HIT_NODE_NAME), CC_CALLBACK_1(Character::onHurt, this)) };
    this->addChild(hitBox);
    _hitBox = hitBox;
    
    // HPを生成
    HitPoint* hitPoint { HitPoint::create(1, CC_CALLBACK_0(Character::onLostHP, this)) };
    CC_SAFE_RETAIN(hitPoint);
    _hitPoint = hitPoint;
    
    // 視野を生成
    Sight* sight { Sight::create(this) };
    CC_SAFE_RETAIN(sight);
    _sight = sight;
    
    BattleCharacterData* battleData { BattleCharacterData::create(data.chara_id) };
    CC_SAFE_RETAIN(battleData);
    _battleData = battleData;
	
    return true;
}

// キャラクタIDを取得
int Character::getCharacterId() const { return _charaId; }

// 自身のキャラクターデータを返す
CharacterData Character::getCharacterData() const { return CharacterData(_charaId, this->getObjectId(), _location); }

// バトル用のデータを返す
BattleCharacterData* Character::getBattleCharacterData() const { return _battleData; }

// 戦闘用攻撃判定を返す
AttackBox* Character::getBattleAttackBox() const { return _battleAttackBox; }

// キャラクターの向きを変える
void Character::setDirection(const Direction& direction)
{
    this->setDirection(direction, true);
}

// キャラクターの向きを変える
// アニメーション再生を中断させるかを指定できる
void Character::setDirection(const Direction& direction, bool stopAnimation)
{
    if(!this->isChangeableDirection(direction)) return;
    
    MapObject::setDirection(direction);
    
    if(!stopAnimation) return;
    
    _csNode->play(AnimationName::getTurn(direction));
}

// AIを一時停止
void Character::pauseAi()
{
    if (!_movePattern) return;
    this->clearCommandQueue();
    _movePattern->pause();
}

// AIを再開
void Character::resumeAi()
{
    if (!_movePattern) return;
    _movePattern->resume();
}

// 方向を指定して歩行させる
bool Character::walkBy(const vector<Direction>& directions, function<void(bool)> cb, float speed, bool back, bool ignoreCollision)
{
    function<void(bool)> callback { [this, cb](bool canMove){
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(MapObject::DURATION_MOVE_ONE_GRID), CallFunc::create([this] {
            if (this->isMoving()) return;
            if (this->isInAttackMotion()) return;
            this->setDirection(this->getDirection());
        })));
        cb(canMove);
    }};
    
    if (!MapObject::moveBy(directions, callback, speed * _speed, ignoreCollision)) return false;
    
    // 方向を変える
    Direction direction { back ? directions.back().getOppositeDirection() : directions.back() };
    this->setDirection(direction, false);
    
    // 歩行アニメーション
    this->stamp(direction, speed * _speed);
    
    return true;
}


// 周りを見渡す
void Character::lookAround(function<void()> callback)
{
    this->setDirection(this->getDirection().convertToWorldDirection(Direction::RIGHT));
    this->runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([this] {
        this->setDirection(this->getDirection().convertToWorldDirection(Direction::UP));
    }), DelayTime::create(1.f), CallFunc::create([callback] {
        callback();
    }), nullptr));
    
}

// 周りを見渡す
void Character::lookAround(function<void()> callback, Direction direction)
{
    this->setDirection(direction);
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.5f), CallFunc::create(callback)));
}

#pragma mark -
#pragma mark CSNode

// アニメーションが再生されているか
bool Character::isAnimationPlaying() const
{
    if (!_csNode) return false;
    return _csNode->isPlaying();
}

// アニメーションを再生
void Character::playAnimation(const string& name, float speed, bool loop)
{
    if (!_csNode) return;
    _csNode->play(name, speed, loop);
}

void Character::playAnimationIfNotPlaying(const string& name, float speed)
{
    if (!_csNode) return;
    _csNode->playIfNotPlaying(name, speed);
}

void Character::playAnimation(const string& name, function<void(Character*)> callback)
{
    if (!_csNode) return;
    _csNode->play(name, [this, callback]{ callback(this); });
}

#pragma mark -
#pragma mark TerrainState

// 足踏み
void Character::stamp(const Direction direction, float ratio)
{
    if (!_terrainState) return;
    _terrainState->stamp(this, direction, ratio);
}

bool Character::isRunnable() const
{
    if (!_terrainState) return true;
    return _terrainState->isRunnable();
}

bool Character::consumeStaminaWalking() const
{
    if (!_terrainState) return false;
    return _terrainState->consumeStaminaWalking();
}

float Character::getStaminaConsumptionRatio() const
{
    if (!_terrainState) return 1.f;
    return _terrainState->getStaminaConsumptionRatio();
}

#pragma mark -
#pragma mark Battle
// 攻撃モーション中状態にする
void Character::beInAttackMotion(bool isInAttackMotion)
{
    _isInAttackMotion = isInAttackMotion;
}

// 攻撃モーション中か
bool Character::isInAttackMotion() const
{
    return _isInAttackMotion;
}

// 自分の攻撃が誰かに当たった時
void Character::onAttackHitted(MapObject* hittedObject)
{
    
}

// 攻撃を受けた時
void Character::onHurt(int damage)
{
    if (!_hitPoint) return;
    _hitPoint->reduce(damage);
    
    if (_battle) {
        HurtCommand* command { HurtCommand::create() };
        this->pushCommand(command);
    }
}

// 対象を攻撃できるか
bool Character::canAttack(MapObject* target) const
{
    if (!_battle) return false;
    return true;
}

#pragma mark -
#pragma mark HitBox
void Character::enableHit(bool enableHit)
{
    if (!_objectList) return;
    
    if (enableHit) {
        _objectList->getAttackDetector()->addHitBox(_hitBox);
        _hitBox->setVisible(true);
    } else {
        _objectList->getAttackDetector()->removeHitBox(_hitBox);
        _hitBox->setVisible(false);
    }
}

#pragma mark -
#pragma mark Sight

bool Character::isInSight(MapObject* mapObject)
{
    if (!_sight) return false;
    
    return _sight->isIn(mapObject, _objectList);
}

#pragma mark -
#pragma mark Interface

// 毎フレーム処理
void Character::update(float delta)
{
    MapObject::update(delta);
    
    if (_movePattern) {
        _movePattern->update(delta);
    }
}

// マップに配置された時
void Character::onEnterMap()
{
    this->scheduleUpdate();
    
    if (_objectList) {
        _objectList->getCollisionDetector()->addIgnorableCollision(this->getCollision());
    }
    this->enableHit(true);
    
    this->setDirection(this->getDirection());
    
    if (DungeonSceneManager::getInstance()->isEventRunning()) {
        this->onEventStart();
    } else {
        if (_movePattern) _movePattern->start();
    }
}

// マップから削除された時
void Character::onExitMap()
{
    if (_objectList) {
        _objectList->getCollisionDetector()->removeIgnorableCollision(this->getCollision());
    }
    this->enableHit(false);
    
    this->unscheduleUpdate();
}

// 主人公一行に参加した時
void Character::onJoinedParty()
{
    if (_objectList) {
        _objectList->getCollisionDetector()->removeIgnorableCollision(this->getCollision());
    }
}

// 主人公一行から抜けた時
void Character::onQuittedParty()
{
    if (_objectList) {
        _objectList->getCollisionDetector()->addIgnorableCollision(this->getCollision());
    }
}

// 主人公一行が動いた時
void Character::onPartyMoved()
{
    if (_movePattern) _movePattern->onPartyMoved();
}

// 調べられた時
void Character::onSearched(MapObject* mainChara)
{
    // 主人公の反対の方向を向かせる（向かいあわせる）
    this->setDirection(mainChara->getDirection().getOppositeDirection());
}

// イベント開始時
void Character::onEventStart()
{
    this->setPaused(true);
    this->getActionManager()->pauseTarget(this);
}

// イベント終了時
void Character::onEventFinished()
{
    this->setPaused(false);
    if (_movePattern && _movePattern->isPaused()) _movePattern->resume();
    this->getActionManager()->resumeTarget(this);
}

// バトル開始時
void Character::onBattleStart(Battle* battle)
{
    MapObject::onBattleStart(battle);

    if (_battleData) {
        _hitPoint->setMax(_battleData->getHitPoint());
        _speed = _battleData->getSpeedRatio();
    }
    
    if (_battleAttackBox) {
        this->removeChild(_battleAttackBox);
    }
    
    AttackBox* box { AttackBox::create(this, _csNode->getCSChild(CS_BATTLE_ATTACK_NODE_NAME), CC_CALLBACK_1(Character::onAttackHitted, this)) };
    if (box) {
        _objectList->getAttackDetector()->addAttackBox(box);
        this->addChild(box);
        _battleAttackBox = box;
    }
}

// バトル終了時
void Character::onBattleFinished()
{
    MapObject::onBattleFinished();
    
    _hitPoint->setMax(1);
    _speed = 1.f;
    
    if (_battleAttackBox) {
        this->removeChild(_battleAttackBox);
        _battleAttackBox = nullptr;
    }
    _objectList->getAttackDetector()->removeAttackBox(_battleAttackBox);
}

#pragma mark -
#pragma mark AnimationName
string Character::AnimationName::getTurn(const Direction& direction)
{
    return direction.getDowncaseString();
}

string Character::AnimationName::getWalk(const Direction& direction)
{
    return "walk_" + direction.getDowncaseString();
}

string Character::AnimationName::getSwim(const Direction& direction)
{
    return "swim_" + direction.getDowncaseString();
}

string Character::AnimationName::getAttack(const string& name, const Direction& direction)
{
    return name + "_"+ direction.getDowncaseString();
}
