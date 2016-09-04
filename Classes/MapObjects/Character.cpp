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

#include "MapObjects/MovePatterns/MovePattern.h"
#include "MapObjects/MovePatterns/MovePatternFactory.h"

#include "MapObjects/TerrainState/TerrainState.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
Character::Character() { FUNCLOG }

// デストラクタ
Character::~Character()
{
    FUNCLOG
    
    CC_SAFE_RELEASE(this->movePattern);
}

// 初期化
bool Character::init(const CharacterData& data)
{
	if(!MapObject::init()) return false;
    
	// 生成時の情報をセット
    this->charaId = data.chara_id;
	this->location = data.location;
    this->setObjectId(data.obj_id);
    
    CSNode* csNode { CSNode::create(data.getCsbFilePath()) };
    if(!csNode) return false;
    this->csNode = csNode;
    this->addChild(csNode);
    
    if(!this->movePattern)
    {
        // 動きのアルゴリズムを生成
        MovePatternFactory* factory { MovePatternFactory::create() };
        CC_SAFE_RETAIN(factory);
        this->movePattern = factory->createMovePattern(data.move_pattern, this);
        CC_SAFE_RETAIN(this->movePattern);
        CC_SAFE_RELEASE(factory);
    }
    
    // サイズ、衝突判定範囲をセット
    this->setContentSize(Size(GRID * 2, GRID * 2));
    this->setCollisionRect(Rect(0, 0, this->getContentSize().width, this->getContentSize().height / 2));
	
    return true;
}

// キャラクタIDを取得
int Character::getCharacterId() const {return this->charaId;}

// 自身のキャラクターデータを返す
CharacterData Character::getCharacterData() const { return CharacterData(this->charaId, this->getObjectId(), this->location); }

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
    
    this->csNode->play(AnimationName::getTurn(direction));
}

// AIを一時停止
void Character::pauseAi()
{
    if(!this->movePattern) return;
    
    this->clearDirectionsQueue();
    
    this->movePattern->pause();
}

// AIを再開
void Character::resumeAi()
{
    if(!this->movePattern) return;
    
    this->movePattern->resume();
}

// 方向を指定して歩行させる
bool Character::walkBy(const Direction& direction, function<void()> onWalked, const float ratio, const bool back)
{
    vector<Direction> directions {direction};
    
    return this->walkBy(directions, onWalked, ratio, back);
}

// 方向を指定して歩行させる
bool Character::walkBy(const vector<Direction>& directions, function<void()> onWalked, const float ratio, const bool back)
{
    if(!MapObject::moveBy(directions, onWalked, ratio)) return false;
    
    // 方向を変える
    Direction direction { back ? directions.back().getOppositeDirection() : directions.back() };
    this->setDirection(direction, false);
    
    // 歩行アニメーション
    this->stamp(direction, ratio);
    
    return true;
}

// 方向とマス数してで歩行させる
void Character::walkBy(const Direction& direction, const int gridNum, function<void(bool)> callback, const float ratio, const bool back)
{
    vector<Direction> directions {direction};
    
    this->walkBy(directions, gridNum, callback, ratio, back);
}

// 方向とマス数指定で歩行させる
void Character::walkBy(const vector<Direction>& directions, const int gridNum, function<void(bool)> callback, const float ratio, const bool back)
{
    if(directions.empty() || this->isMoving()) return;
    
    deque<vector<Direction>> directionsQueue {};
    
    // 方向をキューに登録
    for(int i { 0 }; i < gridNum; i++)
    {
        directionsQueue.push_back(directions);
    }
    
    // キューに登録した歩行を実行
    this->walkByQueue(directionsQueue, callback, ratio, back);
}

// キューで歩行させる
void Character::walkByQueue(deque<Direction> directionQueue, function<void(bool)> callback, const float ratio, const bool back)
{
    if(directionQueue.empty())
    {
        if(callback) callback(true);
        
        return;
    }
    
    deque<vector<Direction>> directionsQueue {};
    
    for(Direction direction : directionQueue)
    {
        directionsQueue.push_back(vector<Direction>({direction}));
    }
    
    this->walkByQueue(directionsQueue, callback, ratio, back);
}

// キューで歩行させる
void Character::walkByQueue(deque<vector<Direction>> directionsQueue, function<void(bool)> callback, const float ratio, const bool back)
{
    // 初回のみ中身が存在するため、空でない時は格納する
    if(!directionsQueue.empty()) this->directionsQueue = directionsQueue;
    
    // キューが空になったら成功としてコールバックを呼び出し
    if(this->directionsQueue.empty())
    {
        if(callback) callback(true);
        
        return;
    }
    
    // 一時停止中かチェック
    if(this->isPaused())
    {
        this->clearDirectionsQueue();
        
        return;
    }
    
    // キューの先頭を実行
    vector<Direction> directions { this->directionsQueue.front() };
    this->directionsQueue.pop_front();
    
    // 移動開始。失敗時はコールバックを失敗として呼び出し
    if(this->walkBy(directions, [callback, ratio, back, this]{this->walkByQueue(deque<vector<Direction>>({}), callback, ratio, back);}, ratio, back)) return;
    
    if(callback) callback(false);
}

// 周りを見渡す
void Character::lookAround(function<void()> callback)
{
    this->setDirection(this->getDirection().convertToWorldDirection(Direction::RIGHT));
    this->runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([this]{this->setDirection(this->getDirection().convertToWorldDirection(Direction::UP));}), DelayTime::create(1.f), CallFunc::create([callback]{callback();}), nullptr));
    
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
    if(!this->csNode) return;
    
    this->csNode->play(name, speed, loop);
}

void Character::playAnimationIfNotPlaying(const string& name, float speed)
{
    if(!this->csNode) return;
    
    this->csNode->playIfNotPlaying(name, speed);
}

#pragma mark -
#pragma mark TerrainState

// 足踏み
void Character::stamp(const Direction direction, float ratio)
{
    if(!_terrainState) return;
    
    _terrainState->stamp(this, direction, ratio);
}

bool Character::isRunnable() const
{
    if(!_terrainState) return true;
    
    return _terrainState->isRunnable();
}

bool Character::consumeStaminaWalking() const
{
    if(!_terrainState) return false;
    
    return _terrainState->consumeStaminaWalking();
}

float Character::getStaminaConsumptionRatio() const
{
    if(!_terrainState) return 1.f;
    
    return _terrainState->getStaminaConsumptionRatio();
}

#pragma mark -
#pragma mark Callback

// マップに配置された時
void Character::onEnterMap()
{
    this->setDirection(this->getDirection());
    
    if(this->movePattern) this->movePattern->start();
    if(DungeonSceneManager::getInstance()->isEventRunning()) this->onEventStart();
}

// 主人公一行が動いた時
void Character::onPartyMoved()
{
    if(this->movePattern) this->movePattern->onPartyMoved();
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
    if(this->movePattern && this->movePattern->isPaused()) this->movePattern->resume();
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
