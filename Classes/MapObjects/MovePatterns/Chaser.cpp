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
#include "MapObjects/DetectionBox/CollisionDetector.h"

#include "Managers/DungeonSceneManager.h"

// 定数
const int Chaser::PATH_FINDING_THRESHOLD { 10 };

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
void Chaser::onPartyMoved()
{
}

void Chaser::setSpeedRatio(float speed)
{
    MovePattern::setSpeedRatio(speed);
    if (_subPattern) _subPattern->setSpeedRatio(speed);
}

// 動かす
void Chaser::move()
{
    if (this->isPaused()) return;
    
    if (!_chara->isMoving() && !_chara->isInAttackMotion()) {
        _chara->setDirection(Direction::convertVec2(this->getMainCharacter()->getPosition() - _chara->getPosition()));
    }

    // サブアルゴリズムに切り替える必要があるか
    if (this->needsShiftToSubPattern()) {
        this->shiftToSubPattern();
        return;
    }
    
    _chara->clearCommandQueue();
    this->moveProc();
}

// 動かす内部実装
void Chaser::moveProc()
{
    // 経路を取得
    deque<Direction> path { this->getPath() };
    
    this->cutPath(path);
    
    Vector<WalkCommand*> commands { WalkCommand::create(path, [this](bool walked) {
        if (walked) {
            this->move();
        } else {
            _chara->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create(CC_CALLBACK_0(Chaser::onStuck, this))));
        }
    }, _speedRatio, false) };
    
    for (WalkCommand* command : commands) {
        _chara->pushCommand(command);
    }
}

// サブアルゴリズムから自身へ移行
void Chaser::shiftFromSubPattern()
{
    _chara->clearCommandQueue();
    this->moveProc();
}

// サブアルゴリズムへ移行
void Chaser::shiftToSubPattern()
{
    _chara->clearCommandQueue();
    _subPattern->setSpeedRatio(_speedRatio);
    _subPattern->move(CC_CALLBACK_0(Chaser::shiftFromSubPattern, this));
}

// サブの移動アルゴリズムに切り替える必要があるか
bool Chaser::needsShiftToSubPattern() const
{
    // 主人公との間に当たり判定がなければtrueを返す
    return !this->getMapObjectList()->getCollisionDetector()->existsCollisionBetween(_chara, this->getMainCharacter());
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

void Chaser::onStuck()
{
    this->move();
}

// マップ移動可能か
bool Chaser::canGoToNextMap() const { return true; }

// 出口までに掛る時間を計算
float Chaser::calcSummonDelay() const
{
    return static_cast<float>(this->getPath().size()) * MapObject::DURATION_MOVE_ONE_GRID / _speedRatio;
}
