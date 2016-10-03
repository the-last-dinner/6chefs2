//
//  MobRandom.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/19.
//
//

#include "MapObjects/MovePatterns/MobRandom.h"

#include "MapObjects/Character.h"
#include "MapObjects/Command/WalkCommand.h"

// 定数
const float MobRandom::MIN_WAIT_DURATION { 2.f };
const float MobRandom::MAX_WAIT_DURATION { 5.f };
const int MobRandom::DISTANCE { 2 };
const float MobRandom::SPEED_RATIO { 0.4f };
const int MobRandom::SCHEDULE_ACTION_TAG { 200 };

// コンストラクタ
MobRandom::MobRandom() { FUNCLOG };

// デストラクタ
MobRandom::~MobRandom() { FUNCLOG };

// 初期化
bool MobRandom::init(Character* chara)
{
    if(!MovePattern::init(chara)) return false;
    
    _homePosition = chara->getGridPosition();
    
    return true;
}

void MobRandom::start()
{
    MovePattern::start();
    
    this->scheduleMove();
}

void MobRandom::pause()
{
    MovePattern::pause();
    
    _chara->stopActionByTag(SCHEDULE_ACTION_TAG);
}

void MobRandom::move()
{
    if(this->isPaused()) return;
    
    // 移動可能な方向のベクタを用意
    vector<Direction> enableDirections {};
    for(Direction direction : Direction::getAll())
    {
        if(_chara->isHit(direction)) continue;
        
        // DISTANCE * 2マス以上離れた場所に移動しようとしていたら無視
        if((_chara->getGridPosition() + direction.getGridVec2()).distance(_homePosition) > DISTANCE * 2) continue;
        
        enableDirections.push_back(direction);
    }
    
    // 移動可能方向がなければ終了
    if (enableDirections.empty())
    {
        this->scheduleMove();
        
        return;
    }
    
    // 移動可能方向からランダムな要素を取り出す
    vector<Direction> detectedDirs {enableDirections[cocos2d::random(0, static_cast<int>(enableDirections.size()) - 1)]};
    
    WalkCommand* command { WalkCommand::create() };
    command->setDirections(detectedDirs);
    command->setSpeed(SPEED_RATIO);
    command->setWalkCallback([this](bool walked) { this->scheduleMove(); });
    
    _chara->pushCommand(command);
}

void MobRandom::scheduleMove()
{
    // 毎回同じ周期で動かすと他のキャラクタと同じタイミングで動いてしまうため、ズレを生じさせる
    float duration { cocos2d::random(MIN_WAIT_DURATION, MAX_WAIT_DURATION) };
    Action* action { Sequence::createWithTwoActions(DelayTime::create(duration), CallFunc::create(CC_CALLBACK_0(MobRandom::move, this))) };
    action->setTag(SCHEDULE_ACTION_TAG);
    _chara->runAction(action);
}
