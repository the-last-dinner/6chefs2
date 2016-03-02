//
//  TrophyNotification.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/05.
//
//

#include "UI/Notification/TrophyNotification.h"

#include "UI/Cloud.h"

// コンストラクタ
TrophyNotification::TrophyNotification() {FUNCLOG};

// デストラクタ
TrophyNotification::~TrophyNotification() {FUNCLOG};

// 初期化
bool TrophyNotification::init(const string& message)
{
    if(!NotificationNode::init(5.f)) return false;
    
    // ラベルを生成
    Label* messageL {Label::createWithTTF("トロフィを獲得しました", Resource::Font::SYSTEM, 20.f)};
    Label* trophyName { Label::createWithTTF(message, Resource::Font::SYSTEM, 25.f) };
    
    // トロフィ生成
    Sprite* trophy { Sprite::createWithSpriteFrameName("trophy_gold.png") };
    float trophyScale { (messageL->getContentSize().height + trophyName->getContentSize().height) * 0.6f / trophy->getContentSize().height};
    trophy->setScale(trophyScale);
    
    // 背景を生成
    Cloud* bg { Cloud::create(Size(trophy->getContentSize().width * trophyScale + max(trophyName->getContentSize().width, messageL->getContentSize().width), trophyName->getContentSize().height)) };
    this->addChild(bg);
    this->addChild(messageL);
    this->addChild(trophyName);
    this->addChild(trophy);
    
    messageL->setPosition(trophy->getContentSize().width * trophyScale / 2, messageL->getContentSize().height / 2 + 5.f);
    trophyName->setPosition(trophy->getContentSize().width * trophyScale / 2, -messageL->getContentSize().height / 2);
    trophy->setPosition(-bg->getContentSize().width / 2 + trophy->getContentSize().width * trophyScale, 0);
    
    this->setDefaultPosition(Point(WINDOW_WIDTH - bg->getContentSize().width / 2, WINDOW_HEIGHT - bg->getContentSize().height / 2));
    
    return true;
}

// 通知
void TrophyNotification::notify(AnimationCallback callback)
{
    NotificationNode::notify(callback);
    
    Point toPosition {this->getDefaultPosition()};
    this->setPosition(toPosition + Vec2(0, 30.f));
    this->setOpacity(0);
    this->runAction(Spawn::createWithTwoActions(FadeIn::create(0.3f), EaseCubicActionOut::create(MoveTo::create(0.3f, toPosition))));
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.3f), CallFunc::create([this, callback]{if(callback) callback(this);})));
}

// 閉じる
void TrophyNotification::close(AnimationCallback callback)
{
    this->runAction(Sequence::createWithTwoActions(Spawn::createWithTwoActions(FadeOut::create(0.3f),
                                                                               EaseCubicActionIn::create(MoveBy::create(0.3f, Vec2(0, 15.f)))),
                                                   CallFunc::create([this, callback]{if(callback) callback(this);})));
}
