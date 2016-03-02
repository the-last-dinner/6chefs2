//
//  NotificationNode.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/05.
//
//

#include "UI/Notification/NotificationNode.h"

// コンストラクタ
NotificationNode::NotificationNode() {};

// デストラクタ
NotificationNode::~NotificationNode() {};

// 初期化
bool NotificationNode::init(const float showingDuration)
{
    if(!Node::init()) return false;
    
    // 表示時間
    this->showingDuration = showingDuration;
    
    // カスケード
    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);
    
    return true;
}

// 表示位置を設定
void NotificationNode::setDefaultPosition(const Point& position)
{
    this->setPosition(position);
    
    this->defaultPosition = position;
}

// 表示位位置を取得
Point NotificationNode::getDefaultPosition() const
{
    return this->defaultPosition;
}

// 通知時に呼び出す
void NotificationNode::notify(AnimationCallback callback)
{
    this->scheduleUpdate();
}

// update
void NotificationNode::update(float delta)
{
    // 表示時間を削っていく
    this->showingDuration -= delta;
}

// 表示時間を取得
float NotificationNode::getShowingDuration() const
{
    if(this->showingDuration < 0) return 0.f;
    
    return this->showingDuration;
}
