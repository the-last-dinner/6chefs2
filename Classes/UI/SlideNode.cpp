//
//  SlideNode.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/27.
//
//

#include "UI/SlideNode.h"

bool SlideNode::init(const Point &inPosition, const Point &outPosition)
{
    if (!Node::init()) return false;
    
    // インスタンス変数の格納
    this->inPosition = inPosition;
    this->outPosition = outPosition;
    
    // アウトポジション等のセット
    this->setPosition(outPosition);
    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);
    
    return true;
}


// スライド時間の設定
void SlideNode::setSlideTime(const float &time)
{
    this->slideTime = time;
}

// スライドイン
void SlideNode::slideIn(AnimationCallback callback)
{
    this->runAction(Sequence::createWithTwoActions(EaseCubicActionOut::create(MoveTo::create(this->slideTime, this->inPosition)), CallFunc::create([this, callback]
    {
        if(callback) callback(this);
    })));
    
}

// スライドアウト
void SlideNode::slideOut(AnimationCallback callback)
{
    this->runAction(Sequence::createWithTwoActions(EaseCubicActionOut::create(MoveTo::create(this->slideTime, this->outPosition)), CallFunc::create([this, callback]
    {
        if(callback) callback(this);
    })));
}