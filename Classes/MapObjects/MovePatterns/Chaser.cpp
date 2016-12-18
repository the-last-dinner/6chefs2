//
//  Chaser.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#include "MapObjects/MovePatterns/Chaser.h"

#include "MapObjects/Character.h"
#include "MapObjects/Command/WalkCommand.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/MovePatterns/CheapChaser.h"
#include "MapObjects/PathFinder/PathFinder.h"

#include "Managers/DungeonSceneManager.h"

// 定数
const int Chaser::PATH_FINDING_THRESHOLD { 10 };
const int Chaser::SHIFT_PATTERN_THRESHOLD { 10 };

// コンストラクタ
Chaser::Chaser() { FUNCLOG }

// デストラクタ
Chaser::~Chaser()
{
    FUNCLOG
    CC_SAFE_RELEASE_NULL(_subPattern);
}

// 初期化
bool Chaser::init(Character* character)
{
    if (!MovePattern::init(character)) return false;
    
    // サブアルゴリズム
    CheapChaser* sub { CheapChaser::create(_chara) };
    CC_SAFE_RETAIN(sub);
    _subPattern = sub;
    
    return true;
}

// 追跡開始
void Chaser::start()
{
    MovePattern::start();
    
    this->move();
}

// 一時停止
void Chaser::pause()
{
    MovePattern::pause();
    
    // サブアルゴリズムに対しても適用
    _subPattern->pause();
}

// 追跡再開
void Chaser::resume()
{
    MovePattern::resume();
    
    this->move();
}

// 主人公一行が動いた時
void Chaser::onPartyMoved() {}

// 動かす
void Chaser::move()
{
    if (this->isPaused()) return;
    
    // 経路を取得
    deque<Direction> path { this->getPath() };
    
    // 経路がない場合は、主人公に触れたとして無視。あとはMapObjectListがイベントを発動してゲームオーバー
    if (path.size() == 0) return;
    
    // サブアルゴリズムに切り替える必要があるか
    if (this->needsShiftToSubPattern(path)) {
        this->shiftToSubPattern();
        return;
    }
    
    this->cutPath(path);
    
    Vector<WalkCommand*> commands { WalkCommand::create(path, [this](bool walked) { if (walked) this->move(); }, _speedRatio, false) };
    
    for (WalkCommand* command : commands) {
        _chara->pushCommand(command);
    }
}

// サブアルゴリズムから自身へ移行
void Chaser::shiftFromSubPattern()
{
    // 経路を取得
    deque<Direction> path { this->getPath() };
    
    // 経路をカット
    this->cutPath(path);
    
    Vector<WalkCommand*> commands { WalkCommand::create(path, [this](bool walked) { if (walked) this->move(); }, _speedRatio, false) };
    
    for (WalkCommand* command : commands) {
        _chara->pushCommand(command);
    }
}

// サブアルゴリズムへ移行
void Chaser::shiftToSubPattern()
{
    _subPattern->setSpeedRatio(_speedRatio);
    _subPattern->move(CC_CALLBACK_0(Chaser::shiftFromSubPattern, this));
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
    if (path.size() < PATH_FINDING_THRESHOLD) return;
    
    // 必要分だけ残す
    for (int i {0}; i < path.size() - PATH_FINDING_THRESHOLD; i++) {
        path.pop_back();
    }
}

// 経路を取得
deque<Direction> Chaser::getPath() const
{
    PathFinder* pathFinder { DungeonSceneManager::getInstance()->getMapObjectList()->getPathFinder() };
    deque<Direction> path { pathFinder->getPath(_chara, this->getMainCharacter()->getGridCollisionRect().origin) };
    
    return path;
}

// マップ移動可能か
bool Chaser::canGoToNextMap() const { return true; };

// 出口までに掛る時間を計算
float Chaser::calcSummonDelay() const
{
    return static_cast<float>(this->getPath().size()) * MapObject::DURATION_MOVE_ONE_GRID / _speedRatio;
}
