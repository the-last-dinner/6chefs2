//
//  MapNameNotification.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/04.
//
//

#include "UI/Notification/MapNameNotification.h"

#include "UI/Cloud.h"

// 定数
const float MapNameNotification::ANIMATION_DURATION {1.f};

// コンストラクタ
MapNameNotification::MapNameNotification() {FUNCLOG};

// デストラクタ
MapNameNotification::~MapNameNotification() {FUNCLOG};

// 初期化
bool MapNameNotification::init(const string& message)
{
    if(!NotificationNode::init(5.f)) return false;
    
    // ラベルを生成
    Label* label { Label::createWithTTF(message, Resource::Font::SYSTEM, 30.f) };
    
    // 背景を生成
    Cloud* bg { Cloud::create(label->getContentSize()) };
    this->addChild(bg);
    this->addChild(label);
    
    this->setContentSize(bg->getContentSize());
    this->setDefaultPosition(Point(this->getContentSize().width / 2 , WINDOW_HEIGHT - this->getContentSize().height / 2));
    
    return true;
}

// 通知
void MapNameNotification::notify(AnimationCallback callback)
{
    NotificationNode::notify(callback);
    
    Point toPosition {this->getDefaultPosition()};
    this->setOpacity(0);
    this->setPosition(toPosition - Vec2(0, 30.f));
    this->runAction(Spawn::createWithTwoActions(FadeIn::create(ANIMATION_DURATION), EaseCubicActionOut::create(MoveTo::create(ANIMATION_DURATION, toPosition))));
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(ANIMATION_DURATION), CallFunc::create([this, callback]{if(callback) callback(this);})));
}

// 閉じる
void MapNameNotification::close(AnimationCallback callback)
{
    this->runAction(Sequence::createWithTwoActions(Spawn::createWithTwoActions(FadeOut::create(ANIMATION_DURATION),
                                                                 EaseCubicActionIn::create(MoveBy::create(ANIMATION_DURATION, Vec2(0, -15.f)))),
                                                   CallFunc::create([this, callback]{if(callback) callback(this);})));
}
