//
//  SlideNode.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/27.
//
//

#ifndef SlideNode_h
#define SlideNode_h

#include "define.h"

class SlideNode : public Node
{
public:
    //定数
    using AnimationCallback = function<void(SlideNode*)>;
    
    // クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(SlideNode, const Point&, const Point&)
protected:
    SlideNode(){FUNCLOG};
    virtual ~SlideNode(){FUNCLOG};
    
    // インスタンス変数
protected:
    Point inPosition {Point::ZERO};
    Point outPosition {Point::ZERO};
    float slideTime {0.3f};
    
    // インスタンスメソッド
protected:
    bool init(const Point& inPosition, const Point& outPosition);
public:
    void setSlideTime(const float& time);
    virtual void slideIn(AnimationCallback callback = nullptr);
    virtual void slideOut(AnimationCallback callback = nullptr);
    
};

#endif /* SlideNode_h */
