//
//  Scouter.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#include "MapObjects/MovePatterns/Scouter.h"

#include "Algorithm/PathFinder.h"

#include "MapObjects/MovePatterns/Chaser.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/PathObject.h"

#include "Managers/DungeonSceneManager.h"

#include "Models/Sight.h"

// コンストラクタ
Scouter::Scouter() {FUNCLOG};

// デストラクタ
Scouter::~Scouter()
{
    FUNCLOG
    
    CC_SAFE_RELEASE_NULL(this->finder);
    CC_SAFE_RELEASE_NULL(this->sight);
    CC_SAFE_RELEASE_NULL(this->subPattern);
    
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
};

// 初期化
bool Scouter::init(Character* character)
{
    if(!MovePattern::init(character)) return false;
    
    PathFinder* finder { PathFinder::create(DungeonSceneManager::getInstance()->getMapSize()) };
    CC_SAFE_RETAIN(finder);
    this->finder = finder;
    
    Sight* sight {Sight::create(character)};
    CC_SAFE_RETAIN(sight);
    this->sight = sight;
    
    return true;
}

// 移動開始
void Scouter::start()
{
    if(!this->isPaused()) return;
    
    MovePattern::start();
    
    if(!this->chara->isMoving()) this->move(this->startPathId);
    if(!Director::getInstance()->getScheduler()->isScheduled(CC_SCHEDULE_SELECTOR(Scouter::update), this)) Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

// 停止
void Scouter::setPaused(bool paused)
{
    MovePattern::setPaused(paused);
    
    if(paused) Director::getInstance()->getScheduler()->unscheduleUpdate(this);
    
    // サブアルゴリズムに対しても適用
    if(this->subPattern) this->subPattern->setPaused(paused);
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
    if(this->isPaused()) return;
    
    // サブパターンが生成されていればそちらに移動を任せる
    if(this->subPattern)
    {
        this->shiftToSubPattern();
        
        return;
    }
    
    // 目的地までの経路を取得
    PathObject* destObj { this->getMapObjectList()->getPathObjectById(pathObjId) };
    
    this->startPathId = destObj->getPathId();
    
    function<void()> walkCallback { [this, destObj]{ this->move(destObj->getNextId()); } };
    
    if(destObj->needsLookingAround()) walkCallback = [this, destObj, walkCallback]
    {
        if(this->subPattern)
        {
            walkCallback();
            
            return;
        }
        
        this->chara->lookAround(walkCallback, destObj->getLookDirection());
    };
    
    if(!this->chara->isMoving()) this->chara->walkByQueue(this->getPath(destObj), [walkCallback](bool _){ walkCallback(); }, destObj->getSpeedRatio(), false);
}

// 指定経路オブジェクトまでの経路を取得
deque<Direction> Scouter::getPath(PathObject* pathObject)
{
    deque<Direction> path {this->finder->getPath(this->chara->getGridRect(), this->getMapObjectList()->getGridCollisionRects({this->chara, this->getMainCharacter()}), pathObject->getGridPosition())};
    
    return path;
}

// 毎フレーム呼ばれる視界チェックメソッド
void Scouter::update(float _)
{
    if(!this->sight->isIn(this->getMainCharacter(), this->getMapObjectList())) return;
    
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
    
    this->onTargetCameIntoSight();
}

// サブパターンに切り替える
void Scouter::shiftToSubPattern()
{
    MovePattern::setPaused(true);
    
    this->subPattern->start();
    
    this->chara->reaction();
    SoundManager::getInstance()->playSE("agaa.mp3");
}

// 主人公が視界に入った時
void Scouter::onTargetCameIntoSight()
{
    this->chara->clearDirectionsQueue();
    
    // サブパターン生成
    Chaser* sub { Chaser::create(this->chara) };
    sub->setSpeedRatio(this->speedRatio);
    CC_SAFE_RETAIN(sub);
    this->subPattern = sub;
    
    // 移動中じゃなけられば、サブパターンでの移動開始
    if(this->chara->isMoving()) return;
    this->shiftToSubPattern();
}
