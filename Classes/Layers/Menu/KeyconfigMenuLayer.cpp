//
//  KeyconfigMenuLayer.cpp
//  6chef2
//
//  Created by Kohei Asami on 2016/03/12.
//
//

#include "Layers/Menu/KeyconfigMenuLayer.h"

// コンストラクタ
KeyconfigMenuLayer::KeyconfigMenuLayer() { FUNCLOG };

// デストラクタ
KeyconfigMenuLayer::~KeyconfigMenuLayer() { FUNCLOG };

// 初期化
bool KeyconfigMenuLayer::init()
{
    if(!MenuLayer::init(1, 3)) return false;
    
    // 外枠を生成
    Sprite* baseFrame {Sprite::create()};
    baseFrame->setColor(Color3B::BLACK);
    baseFrame->setTextureRect(Rect(0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
    baseFrame->setPosition(WINDOW_CENTER);
    
    return true;
}