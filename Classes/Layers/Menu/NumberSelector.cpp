//
//  NumberSelector.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/29.
//
//

#include "Layers/Menu/NumberSelector.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// 初期化
bool NumberSelector::init(SpriteUtils::Square& square)
{
    Point indexSize {Point(3,4)};
    int buttonCount { 11 };
    
    // ウインドウ作成
    SpriteUtils::Margin margin = SpriteUtils::Margin(3.0);
    Sprite* window = SpriteUtils::getSquareSprite(square, margin);
    window->setColor(Color3B::BLACK);
    window->setOpacity(0);
    Size windowSize = window->getContentSize();
    this->addChild(window);
    
    if (!MenuLayer::init(indexSize, buttonCount, window)) return false;
    
    // ウインドウの中にボタンの生成
    float buttonMargin = 6.f;
    float buttonSize = windowSize.width / indexSize.x;
    for(int i=0; i<=10; i++)
    {
        // ボタンを作成
        DrawNode* circle = DrawNode::create();
        circle->drawDot(Vec2(0,0), buttonSize/2 - buttonMargin, Color4F::Color4F(0.50,0,0,1));
        circle->setPosition((i%(int)indexSize.x) * buttonSize + buttonSize / 2, windowSize.height - (floor(i/indexSize.x)) * buttonSize - buttonSize/2);
        circle->setCascadeOpacityEnabled(true);
        window->addChild(circle);
        
        // ボタンラベルを作成
        Label* label = Label::createWithTTF(i < 10 ? to_string(this->indexToLabel(i)) : "C", "fonts/cinecaption2.28.ttf", 24);
        label->setPosition(circle->getContentSize().width/2, circle->getContentSize().height/2);
        label->setColor(Color3B::WHITE);
        circle->addChild(label);
        
        // メニューオブジェクトに登録
        circle->setTag(i);
        circle->setOpacity(128);
        this->menuObjects.push_back(circle);
    }
    
    // 初期セット
    this->onIndexChanged(0, false);
    
    return true;
}

void NumberSelector::show()
{
    this->listenerKeyboard->setEnabled(true);
    this->setVisible(true);
}

void NumberSelector::hide()
{
    this->listenerKeyboard->setEnabled(false);
    this->setVisible(false);
}

// インデックスが変わった時
void NumberSelector::onIndexChanged(int newIdx, bool sound)
{
    if(sound)
    {
        SoundManager::getInstance()->playSE("cursorMove.mp3");
    }
    for(Node* obj : this->menuObjects)
    {
        if(obj->getTag() == newIdx)
        {
            obj->runAction(FadeTo::create(0.2f, 255));
            obj->runAction(ScaleTo::create(0.2f, 1.15f));
        }
        else
        {
            obj->runAction(FadeTo::create(0.2f, 128));
            obj->runAction(ScaleTo::create(0.2f, 1.0f));
        }
    }
}

// 数字が選ばれた時
void NumberSelector::onEnterKeyPressed(int idx)
{
    if (this->onNumberSelectorSelected)
    {
        this->onNumberSelectorSelected(this->indexToLabel(idx));
    }
}

// キャンセルされた時
void NumberSelector::onMenuKeyPressed()
{
    if(this->onNumberSelectorCanceled)
    {
        this->onNumberSelectorCanceled();
    }
}

// インデックスの値からラベルの値に変換
int NumberSelector::indexToLabel(int idx)
{
    return idx < 10 ? (idx+1)%10 : idx;
}
