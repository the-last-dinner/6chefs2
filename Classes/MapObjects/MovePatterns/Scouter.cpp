//
//  Scouter.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#include "MapObjects/MovePatterns/Scouter.h"

#include "MapObjects/Command/WalkCommand.h"
#include "MapObjects/MovePatterns/Chaser.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/PathObject.h"
#include "MapObjects/PathFinder/PathFinder.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
Scouter::Scouter() { FUNCLOG }

// デストラクタ
Scouter::~Scouter()
{
    FUNCLOG
    
    CC_SAFE_RELEASE_NULL(_subPattern);
}

// 初期化
bool Scouter::init(Character* character)
{
    if (!MovePattern::init(character)) return false;
    
    return true;
}

// 移動開始
void Scouter::start()
{
    MovePattern::start();
    
    if (!_chara->isMoving()) this->move(_startPathId);
}

// 一時停止
void Scouter::pause()
{
    MovePattern::pause();
    
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
    
    // サブアルゴリズムに対しても適用
    if (_subPattern) _subPattern->pause();
}

// 再開
void Scouter::resume()
{
    MovePattern::resume();
    
    if (!_chara->isMoving()) this->move(_startPathId);
}

// 主人公一行が移動した時
void Scouter::onPartyMoved() {}

// 次のマップへ移動するか
bool Scouter::canGoToNextMap() const { return false; };

// 次のマップへ出現するまでの時間を取得
float Scouter::calcSummonDelay() const { return 0.f; };

// 動かす
void Scouter::move(const int pathObjId)
{
    if (this->isPaused()) return;
    
    // サブパターンが生成されていればそちらに移動を任せる
    if (_subPattern) {
        this->shiftToSubPattern();
        return;
    }
    
    // 目的地までの経路を取得
    PathObject* destObj { this->getMapObjectList()->getPathObjectById(pathObjId) };
    
    _startPathId = destObj->getPathId();
    
    function<void()> walkCallback { [this, destObj] { this->move(destObj->getNextId()); } };
    
    if (destObj->needsLookingAround()) walkCallback = [this, destObj, walkCallback] {
        if (_subPattern) {
            walkCallback();
            return;
        }
        
        _chara->lookAround(walkCallback, destObj->getLookDirection());
    };
    
    Vector<WalkCommand*> commands { WalkCommand::create(this->getPath(destObj), [walkCallback](bool _) {
        walkCallback();
    }, destObj->getSpeedRatio(), false) };
    
    _chara->clearCommandQueue();
    for (WalkCommand* command : commands) {
        _chara->pushCommand(command);
    }
}

// 指定経路オブジェクトまでの経路を取得
deque<Direction> Scouter::getPath(PathObject* pathObject)
{
    PathFinder* finder { DungeonSceneManager::getInstance()->getMapObjectList()->getPathFinder() };
    
    deque<Direction> path { finder->getPath(_chara, pathObject->getGridPosition()) };
    
    return path;
}

// サブパターンに切り替える
void Scouter::shiftToSubPattern()
{
    MovePattern::pause();
    _chara->clearCommandQueue();
    _subPattern->start();
    
    _chara->reaction();
    SoundManager::getInstance()->playSE("agaa.mp3");
}

// 主人公が視界に入った時
void Scouter::onTargetCameIntoSight()
{
    _chara->clearCommandQueue();
    
    // サブパターン生成
    Chaser* sub { Chaser::create(_chara) };
    sub->setSpeedRatio(_speedRatio);
    CC_SAFE_RETAIN(sub);
    _subPattern = sub;
    
    // サブパターンでの移動開始
    this->shiftToSubPattern();
}

#pragma mark -
#pragma mark Interface

void Scouter::update(float delta)
{
    if (_subPattern) return;
    
    if (!_chara->isInSight(this->getMainCharacter())) return;
    
    this->onTargetCameIntoSight();
}
