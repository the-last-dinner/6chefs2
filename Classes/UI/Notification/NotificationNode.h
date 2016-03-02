//
//  NotificationNode.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/05.
//
//

#ifndef NotificationNode_h
#define NotificationNode_h

#include "Common.h"

class NotificationNode : public Node
{
// 定数
public:
    using AnimationCallback = function<void(NotificationNode*)>;
// インスタンス変数
private:
    float showingDuration { 0.f };
    Point defaultPosition { Point::ZERO };
    
// インスタンスメソッド
protected:
    NotificationNode();
    ~NotificationNode();
    void setDefaultPosition(const Point& position);
    Point getDefaultPosition() const;
public:
    virtual bool init(const float showingDuration);
    virtual void notify(AnimationCallback callback);
    virtual void close(AnimationCallback callback) = 0;
    virtual float getShowingDuration() const;
    void update(float delta);
};

#endif /* NotificationNode_h */
