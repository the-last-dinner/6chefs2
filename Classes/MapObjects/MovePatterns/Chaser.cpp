//
//  Chaser.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#include "MapObjects/MovePatterns/Chaser.h"

#include "MapObjects/Character.h"
#include "MapObjects/MapObjectList.h"

#include "MapObjects/MovePatterns/CheapChaser.h"

#include "Algorithm/PathFinder.h"

#include "Managers/DungeonSceneManager.h"

// 定数
const int Chaser::PATH_FINDING_THRESHOLD { 10 };
const int Chaser::SHIFT_PATTERN_THRESHOLD { 10 };

// コンストラクタ
Chaser::Chaser() {FUNCLOG};

// デストラクタ
Chaser::~Chaser()
{
    FUNCLOG
    CC_SAFE_RELEASE_NULL(this->pathFinder);
    CC_SAFE_RELEASE_NULL(this->subPattern);
};

// 初期化
bool Chaser::init(Character* character)
{
    if(!MovePattern::init(character)) return false;
    
    // 経路探索
    PathFinder* pathFinder { PathFinder::create(DungeonSceneManager::getInstance()->getMapSize()) };
    CC_SAFE_RETAIN(pathFinder);
    this->pathFinder = pathFinder;
    
    // サブアルゴリズム
    CheapChaser* sub { CheapChaser::create(this->chara) };
    CC_SAFE_RETAIN(sub);
    this->subPattern = sub;
    
    return true;
}

// 追跡開始
void Chaser::start()
{
    if(!this->isPaused()) return;
    
    MovePattern::start();
    
    if(!this->chara->isMoving()) this->move();
}

// 停止
void Chaser::setPaused(bool paused)
{
    MovePattern::setPaused(paused);
    
    // サブアルゴリズムに対しても適用
    this->subPattern->setPaused(paused);
}

// 主人公一行が動いた時
void Chaser::onPartyMoved() {}

// 動かす
void Chaser::move()
{
    if(this->isPaused()) return;
    
    // 経路を取得
    deque<Direction> path { this->getPath() };
    
    // 経路がない場合は、主人公に触れたとして無視。あとはMapObjectListがイベントを発動してゲームオーバー
    if(path.size() == 0) return;
    
    // サブアルゴリズムに切り替える必要があるか
    if(this->needsShiftToSubPattern(path))
    {
        this->shiftToSubPattern();
        
        return;
    }
    
    this->cutPath(path);
    
    this->chara->walkByQueue(path, [this](bool reached){if(reached)this->move();}, this->speedRatio, false);
}

// サブアルゴリズムから自身へ移行
void Chaser::shiftFromSubPattern()
{
    // 経路を取得
    deque<Direction> path { this->getPath() };
    
    // 経路をカット
    this->cutPath(path);
 
    this->chara->walkByQueue(path, [this](bool reached){if(reached)this->move();}, this->speedRatio, false);
}

// サブアルゴリズムへ移行
void Chaser::shiftToSubPattern()
{
    this->subPattern->setSpeedRatio(this->speedRatio);
    
    this->subPattern->move(CC_CALLBACK_0(Chaser::shiftFromSubPattern, this));
}

// サブの移動アルゴリズムに切り替える必要があるか
bool Chaser::needsShiftToSubPattern(const deque<Direction>& path)
{
    // 経路のステップ数が閾値以内ならばtrueを返す
    return path.size() <= SHIFT_PATTERN_THRESHOLD;
}

// 経路をカットする
void Chaser::cutPath(deque<Direction>& path)
{
    if(path.size() < PATH_FINDING_THRESHOLD) return;
    
    // 必要分だけ残す
    for(int i {0}; i < path.size() - PATH_FINDING_THRESHOLD; i++)
    {
        path.pop_back();
    }
}

// 経路を取得
deque<Direction> Chaser::getPath() const
{
    deque<Direction> path { this->pathFinder->getPath(this->chara->getGridRect(), this->getMapObjectList()->getGridCollisionRects({this->getMainCharacter(), this->chara}), this->getMainCharacter()->getGridRect().origin) };
    
    return path;
}

// マップ移動可能か
bool Chaser::canGoToNextMap() const { return true; };

// 出口までに掛る時間を計算
float Chaser::calcSummonDelay() const
{
    return this->getPath().size() * MapObject::DURATION_MOVE_ONE_GRID / this->speedRatio;
}
