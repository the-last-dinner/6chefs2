//
//  RandomMove.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/16.
//
//

#include "MapObjects/MovePatterns/RandomMove.h"

#include "MapObjects/Character.h"
#include "MapObjects/Command/WalkCommand.h"

// コンストラクタ
RandomMove::RandomMove() { FUNCLOG }

// デストラクタ
RandomMove::~RandomMove() { FUNCLOG }

// 初期化
bool RandomMove::init(Character* chara)
{
    if(!MovePattern::init(chara)) return false;
    
    return true;
}

// 動かす
void RandomMove::start()
{
    MovePattern::start();
    
    this->move();
}

void RandomMove::move()
{
    if(this->isPaused()) return;
    
    // 移動可能な方向のベクタを用意
    vector<Direction> enableDirections {};
    for(Direction direction : Direction::getAll())
    {
        if(!_chara->isHit(direction)) enableDirections.push_back(direction);
    }
    
    // 移動可能方向がなければ終了
    if (enableDirections.empty()) return;
    
    // 移動可能方向からランダムな要素を取り出す
    vector<Direction> detectedDirs { enableDirections[cocos2d::random(0, static_cast<int>(enableDirections.size()) - 1)] };
    
    WalkCommand* command { WalkCommand::create() };
    command->setDirections(detectedDirs);
    command->setWalkCallback([this](bool walked) { this->move(); });
}
