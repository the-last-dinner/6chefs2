//
//  Fog.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/19.
//
//

#include "Effects/Fog.h"

// 定数
const float Fog::DURATION { 30.f };
const string Fog::IMG_FILE_NAME { "fog.png" };

// コンストラクタ
Fog::Fog() {FUNCLOG};

// デストラクタ
Fog::~Fog() {FUNCLOG};

// 初期化
bool Fog::init()
{
    if(!Node::init()) return false;
    
    // 靄画像を生成
    Sprite* spriteU { Sprite::createWithSpriteFrameName(IMG_FILE_NAME) };
    Sprite* spriteB { Sprite::createWithSpriteFrameName(IMG_FILE_NAME) };
    
    float scale { WINDOW_WIDTH / spriteU->getContentSize().width };
    
    spriteU->setScale(scale);
    spriteB->setScale(scale);
    
    this->addChild(spriteU);
    this->addChild(spriteB);
    
    this->moveSprites(spriteU, spriteB);
    
    return true;
}

void Fog::moveSprites(Sprite* upper, Sprite* bottom)
{
    bottom->setPosition(0, -WINDOW_HEIGHT);
    
    upper->runAction(Sequence::createWithTwoActions(MoveBy::create(DURATION, Vec2(0, WINDOW_HEIGHT)), CallFunc::create([this, upper, bottom]{this->onMoveAnimationFinished(upper, bottom);})));
    
    bottom->runAction(MoveBy::create(DURATION, Vec2(0, WINDOW_HEIGHT)));
}

void Fog::onMoveAnimationFinished(Sprite* upper, Sprite* bottom)
{
    // 上下を入れ替えて再び動かす
    this->moveSprites(bottom, upper);
}
