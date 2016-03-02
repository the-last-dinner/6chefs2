//
//  TrophyNotification.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/05.
//
//

#ifndef TrophyNotification_h
#define TrophyNotification_h

#include "UI/Notification/NotificationNode.h"

class TrophyNotification : public NotificationNode
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(TrophyNotification, const string&)
    
// インスタンスメソッド
private:
    TrophyNotification();
    ~TrophyNotification();
    bool init(const string& message);
public:
    virtual void notify(AnimationCallback callback);
    virtual void close(AnimationCallback callback);
};

#endif /* TrophyNotification_h */
