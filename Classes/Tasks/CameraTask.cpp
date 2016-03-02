//
//  CameraTask.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#include "Tasks/CameraTask.h"

#include "MapObjects/MapObject.h"

#include "Layers/Dungeon/TiledMapLayer.h"

#include "Tasks/FollowType/NeverFollow.h"
#include "Tasks/FollowType/HorizontalFollow.h"
#include "Tasks/FollowType/VerticalFollow.h"
#include "Tasks/FollowType/BothFollow.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
CameraTask::CameraTask() {FUNCLOG};

// デストラクタ
CameraTask::~CameraTask()
{
    FUNCLOG
    
    this->stopFollowing();

    CC_SAFE_RELEASE_NULL(this->follow);
    CC_SAFE_RELEASE_NULL(this->target);
};

// 初期化
bool CameraTask::init()
{
    if(!GameTask::init()) return false;
    
    // マップレイヤ取得
    TiledMapLayer* mapLayer { DungeonSceneManager::getInstance()->getMapLayer() };
    if(!mapLayer) return false;
    this->mapLayer = mapLayer;
    
    // マップの大きさから、縦方向、横方向のカメラ移動を許可するか判定
    Size winSize {Director::getInstance()->getWinSize()};
    Size mapSize {mapLayer->getMapSize()};
    
    // 外枠の大きさ
    Size margin {GRID * 4, GRID * 4};
    
    // マップの大きさ - 想定される外枠の大きさ がウィンドウの大きさよりも大きい時、それぞれの方向へスクロール可能とする
    bool horizontalFollow { mapSize.width - margin.width > winSize.width };
    bool verticalFollow { mapSize.height - margin.height > winSize.height };
    
    // 縦横どちらにもカメラ移動不可の場合
    if(!horizontalFollow && !verticalFollow)
    {
        this->follow = NeverFollow::create();
    }
    // 縦方向のみカメラ移動を許可されている場合
    else if (!horizontalFollow && verticalFollow)
    {
        this->follow = VerticalFollow::create();
    }
    // 横方向のみカメラ移動を許可されている場合
    else if (horizontalFollow && !verticalFollow)
    {
        this->follow = HorizontalFollow::create();
    }
    // 縦横どちらにもカメラ移動可能な時
    else if (horizontalFollow && verticalFollow)
    {
        this->follow = BothFollow::create();
    }
    CC_SAFE_RETAIN(this->follow);
    
    this->resumeFollowing();
    
    return true;
}

// 追いかけるオブジェクトを設定
void CameraTask::setTarget(MapObject* target)
{
    if(!target) return;
    
    CC_SAFE_RELEASE(this->target);
    CC_SAFE_RETAIN(target);
    this->target = target;
    
    this->mapLayer->setPosition(this->follow->getPosition(this->mapLayer->getMapSize(), this->target->getPosition()));
}

// 自動追尾を停止
void CameraTask::stopFollowing()
{
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

// 自動追尾を再開
void CameraTask::resumeFollowing()
{
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

// 指定マス座標が中心に来るように視点移動
void CameraTask::move(const Point& gridPosition, const float duration, function<void()> callback)
{
    // 追尾を停止
    this->stopFollowing();
    
    Point toPosition {-MapUtils::convertToCCPoint(this->mapLayer->getMapSize(), gridPosition, Size(GRID, GRID)) + Director::getInstance()->getWinSize() / 2};
    
    this->runAction(Sequence::createWithTwoActions(TargetedAction::create(this->mapLayer, EaseSineOut::create(MoveTo::create(duration, toPosition))), CallFunc::create(callback)));
}

// 指定マス座標が中心に設定
void CameraTask::setCenter(const Point& gridPosition)
{
    // 追尾を停止
    this->stopFollowing();
    
    this->mapLayer->setPosition(-MapUtils::convertToCCPoint(this->mapLayer->getMapSize(), gridPosition, Size(GRID, GRID)) + Director::getInstance()->getWinSize() / 2);
}

// updateメソッド
void CameraTask::update(float delta)
{
    if(!this->target || !this->follow) return;
    
    this->mapLayer->setPosition(this->follow->getPosition(this->mapLayer->getMapSize(), this->target->getPosition()));
}