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

#include "MapObjects/MapObjectList.h"
#include "MapObjects/DetectionBox/AttackDetector.h"
#include "MapObjects/DetectionBox/CollisionDetector.h"
#include "MapObjects/DetectionBox/HitBox.h"
#include "MapObjects/MovePatterns/MovePattern.h"
#include "MapObjects/MovePatterns/MovePatternFactory.h"
#include "MapObjects/Status/HitPoint.h"
#include "MapObjects/TerrainState/TerrainState.h"

#include "Models/Sight.h"

#include "Managers/DungeonSceneManager.h"

// 定数
const string Character::CS_SPRITE_NODE_NAME { "sprite" };
const string Character::CS_COLLISION_NODE_NAME { "collision" };
const string Character::CS_HIT_NODE_NAME { "hit" };

// コンストラクタ
Character::Character() { FUNCLOG }

// デストラクタ
Character::~Character()
{
    FUNCLOG
    
    CC_SAFE_RELEASE_NULL(_movePattern);
    CC_SAFE_RELEASE_NULL(_hitBox);
    CC_SAFE_RELEASE_NULL(_hp);
    CC_SAFE_RELEASE_NULL(_sight);
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
    HitBox* hitBox { HitBox::create(this, _csNode->getCSChild(CS_HIT_NODE_NAME), CC_CALLBACK_0(Character::onAttackHitted, this)) };
    CC_SAFE_RETAIN(hitBox);
    _hitBox = hitBox;
    
    // HPを生成
    HitPoint* hp { HitPoint::create(1, CC_CALLBACK_0(Character::onLostHP, this)) };
    CC_SAFE_RETAIN(hp);
    _hp = hp;
    
    // 視野を生成
    Sight* sight { Sight::create(this) };
    CC_SAFE_RETAIN(sight);
    _sight = sight;
	
    return true;
}

// キャラクタIDを取得
int Character::getCharacterId() const { return _charaId; }

// 自身のキャラクターデータを返す
CharacterData Character::getCharacterData() const { return CharacterData(_charaId, this->getObjectId(), _location); }

// キャラクターの向きを変える
void Character::setDirection(const Direction& direction)
{
    this->setDirection(direction, true);
}

// キャラクターの向きを変える
// アニメーション再生を中断させるかを指定できる
void Character::setDirection(const Direction& direction, bool stopAnimation)
{
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
    if (!MapObject::moveBy(directions, cb, speed, ignoreCollision)) return false;
    
    // 方向を変える
    Direction direction { back ? directions.back().getOppositeDirection() : directions.back() };
    this->setDirection(direction, false);
    
    // 歩行アニメーション
    this->stamp(direction, speed);
    
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
#pragma mark HitBox

void Character::onAttackHitted()
{
    if (!_hp) return;
    
    _hp->reduce(1);
}

#pragma mark -
#pragma mark HP

void Character::setLostHPCallback(function<void(Character*)> callback)
{
    _onLostHP = callback;
}

void Character::onLostHP()
{
    if (_onLostHP) {
        _onLostHP(this);
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
        _objectList->getAttackDetector()->addHitBox(_hitBox);
    }
    
    this->setDirection(this->getDirection());
    
    if (_movePattern) _movePattern->start();
    if (DungeonSceneManager::getInstance()->isEventRunning()) this->onEventStart();
}

// マップから削除された時
void Character::onExitMap()
{
    if (_objectList) {
        _objectList->getCollisionDetector()->removeIgnorableCollision(this->getCollision());
        _objectList->getAttackDetector()->removeHitBox(_hitBox);
    }
    
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
