//
//  CheapChaser.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#include "MapObjects/MovePatterns/CheapChaser.h"

#include "MapObjects/Character.h"
#include "MapObjects/Command/WalkCommand.h"

// コンストラクタ
CheapChaser::CheapChaser() { FUNCLOG }

// デストラクタ
CheapChaser::~CheapChaser() { FUNCLOG }

// 初期化
bool CheapChaser::init(Character* character)
{
    if (!MovePattern::init(character)) return false;
    
    return true;
}

// 追跡開始
void CheapChaser::start()
{
    MovePattern::start();
    this->move();
}

// パーティが移動した時
void CheapChaser::onPartyMoved()
{
    // もしキャラクタが動いていなければ、動かす
    if (_chara->isMoving()) return;
    this->move();
}

// マップ移動可能か
bool CheapChaser::canGoToNextMap() const
{
    return _chara->canMove(Direction::convertGridVec2(this->getMainCharacter()->getGridCollisionRect().origin - _chara->getGridPosition()));
}

// 次マップへの出現遅延時間を計算
float CheapChaser::calcSummonDelay() const
{
    Vec2 diffVec { this->getMainCharacter()->getGridRect().origin - _chara->getGridPosition() };
    
    // 差が大きい方の要素を距離として時間を計算
    float distance { max(abs(diffVec.x), abs(diffVec.y)) };
    return distance * MapObject::DURATION_MOVE_ONE_GRID * _speedRatio;
}

// 移動
void CheapChaser::move(function<void()> callback)
{
    if (this->isPaused()) return;
    
    WalkCommand* command { WalkCommand::create() };
    command->setDirections(Direction::convertGridVec2(this->getMainCharacter()->getGridRect().origin - _chara->getGridPosition()));
    command->setSpeed(_speedRatio);
    command->setWalkCallback([this, callback](bool walked) {
        if (walked) {
            this->move(callback);
        } else {
            if (callback) callback();
        }
    });
    
    _chara->pushCommand(command);
}
