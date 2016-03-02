//
//  Cloud.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/04.
//
//

#include "UI/Cloud.h"

#include "cocos-ext.h"

// 定数
const float Cloud::MARGIN {25.f};

// コンストラクタ
Cloud::Cloud() {FUNCLOG};

// デストラクタ
Cloud::~Cloud() {FUNCLOG};

// 初期化
bool Cloud::init(const Size& size)
{
    if(!Node::init()) return false;
    
    // カスケード
    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);
    
    Sprite* sprite { Sprite::createWithSpriteFrameName("cloud.png") };
    this->addChild(sprite);
    this->sprite = sprite;
    
    this->setColor(Color3B::BLACK);
    
    // 最小サイズ
    float scale { (size.width + MARGIN * 2) / sprite->getContentSize().width };
    sprite->setScale(scale);
    this->setContentSize(sprite->getContentSize() * scale);
    
    return true;
}

// BlendFuncを設定
void Cloud::setBlendFunc(const BlendFunc& blendFunc)
{
    this->sprite->setBlendFunc(blendFunc);
}