//
//  DisplayImageLayer.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/09.
//
//

#include "Layers/Dungeon/DisplayImageLayer.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// 定数
const string DisplayImageLayer::DISP_IMG_PATH = "disp/";
const float DisplayImageLayer::SHOW_DURATION = 0.5f;
const float DisplayImageLayer::HIDE_DURATION = 0.5f;

// コンストラクタ
DisplayImageLayer::DisplayImageLayer() {FUNCLOG};

//デストラクタ
DisplayImageLayer::~DisplayImageLayer() {FUNCLOG};

// create関数
DisplayImageLayer* DisplayImageLayer::create(const string& imgFileName, const float duration, function<void()> callback)
{
    DisplayImageLayer* p {new(nothrow) DisplayImageLayer()};
    
    if(p && p->init(imgFileName, duration, callback))
    {
        CC_SAFE_RETAIN(p);
        return p;
    }
    else
    {
        delete p;
        p = nullptr;
        return nullptr;
    }
}

// 初期化
bool DisplayImageLayer::init(const string& imageFileName, const float duration, function<void()> callback)
{
    if(!Layer::init()) return false;
    
    this->setCascadeOpacityEnabled(true);
    this->setOpacity(0.f);
    
    // 背景を生成
    Sprite* bg { Sprite::create() };
    bg->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    bg->setColor(Color3B::BLACK);
    bg->setPosition(bg->getContentSize() / 2);
    this->addChild(bg);
    
    // 画像を生成
    string fullPath { FileUtils::getInstance()->fullPathForFilename(Resource::SpriteFrame::BASE_PATH + DISP_IMG_PATH + imageFileName) };
    if(fullPath.size() == 0) return false;
    
    Sprite* img { Sprite::create(Resource::SpriteFrame::BASE_PATH + DISP_IMG_PATH + imageFileName) };
    img->setPosition(WINDOW_CENTER);
    this->addChild(img);
    
    // フェードイン
    this->runAction(Sequence::createWithTwoActions(FadeIn::create(SHOW_DURATION), CallFunc::create([this, callback, duration]{
        // フェードイン後
        // 表示時間が0.f秒以下ならキーボード操作で自身を消す
        if(duration <= 0.f)
        {
            // リスナ生成
            EventListenerKeyboardLayer* listener { EventListenerKeyboardLayer::create() };
            listener->onSpaceKeyPressed = [this, callback]{this->remove(callback);};
            this->addChild(listener);
            
            return;
        }
        
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(duration), CallFunc::create([this, callback]{this->remove(callback);})));
    })));
    
    return true;
}

// 消す
void DisplayImageLayer::remove(function<void()> callback)
{
    this->runAction(Sequence::create(FadeOut::create(HIDE_DURATION), CallFunc::create(callback), RemoveSelf::create(), nullptr));
}