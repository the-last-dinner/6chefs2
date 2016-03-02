//
//  NotificationManager.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/05.
//
//

#include "Managers/NotificationManager.h"

#include "Managers/CsvDataManager.h"
#include "Managers/SoundManager.h"

#include "UI/Notification/MapNameNotification.h"
#include "UI/Notification/TrophyNotification.h"

// 唯一のインスタンス
static NotificationManager* _instance {nullptr};

// インスタンスを取得
NotificationManager* NotificationManager::getInstance()
{
    if(!_instance) _instance = new NotificationManager();
    
    return _instance;
}

// インスタンスの破棄
void NotificationManager::destroy()
{
    delete _instance;
}

// マップ名を通知
void NotificationManager::notifyMapName(const int mapId)
{
    MapNameNotification* n { MapNameNotification::create(CsvDataManager::getInstance()->getMapName(mapId)) };
    Director::getInstance()->getRunningScene()->addChild(n, Priority::NOTIFICATION);
    n->notify(CC_CALLBACK_1(NotificationManager::onNotifyEnterAnimationFinished, this));
}

// トロフィ獲得を通知
void NotificationManager::notifyTrophy(const int trophyId)
{
    TrophyNotification* n { TrophyNotification::create(CsvDataManager::getInstance()->getTrophyName(trophyId)) };
    this->notifications.pushBack(n);
    this->notifyInQueue(n);
}

// キューに残っている通知を再開する。シーン切替完了時に実行
void NotificationManager::notifyRemainsInQueue()
{
    if(this->notifications.empty()) return;
    
    this->notifications.front()->setParent(nullptr);
    
    this->notifyInQueue(this->notifications.front(), false);
}

// トロフィ獲得通知をキューから表示
void NotificationManager::notifyInQueue(NotificationNode* node, const bool sound)
{
    if(this->notifications.front() != node) return;
    
    Director::getInstance()->getRunningScene()->addChild(node, Priority::NOTIFICATION);
    node->notify(CC_CALLBACK_1(NotificationManager::onNotifyEnterAnimationFinished, this));
    if(sound) SoundManager::getInstance()->playSE(Resource::SE::TROPHY_NOTIFICATION);
}

// 通知の表示アニメーション終了時
void NotificationManager::onNotifyEnterAnimationFinished(NotificationNode* node)
{
    node->runAction(Sequence::createWithTwoActions(DelayTime::create(node->getShowingDuration()), CallFunc::create([this, node]{node->close(CC_CALLBACK_1(NotificationManager::onNotifyExitAnimationFinished, this));})));
}

// 通知の閉じるアニメーション終了時
void NotificationManager::onNotifyExitAnimationFinished(NotificationNode* node)
{
    node->removeFromParent();
    
    if(this->notifications.empty()) return;
    
    if(this->notifications.front() != node) return;
    
    this->notifications.erase(0);
    
    if(this->notifications.empty()) return;
    
    this->notifyInQueue(this->notifications.front());
}

