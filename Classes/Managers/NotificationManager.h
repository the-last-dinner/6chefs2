//
//  NotificationManager.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/05.
//
//

#ifndef NotificationManager_h
#define NotificationManager_h

#include "UI/Notification/NotificationNode.h"

class NotificationNode;

class NotificationManager
{
// クラスメソッド
public:
    static NotificationManager* getInstance();
    static void destroy();
    
// インスタンス変数
private:
    Vector<NotificationNode*> notifications {};
    
// インスタンスメソッド
public:
    void notifyMapName(const int mapId);
    void notifyTrophy(const int trophyId);
    void notifyRemainsInQueue();
    
private:
    void notifyInQueue(NotificationNode* node, const bool sound = true);
    void onNotifyEnterAnimationFinished(NotificationNode* node);
    void onNotifyExitAnimationFinished(NotificationNode* node);
};

#endif /* NotificationManager_h */
