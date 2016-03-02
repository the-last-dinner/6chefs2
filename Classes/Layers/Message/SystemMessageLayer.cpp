//
//  SystemMessageLayer.cpp
//  LastSupper
//
//  Created by Kohei on 2015/07/12.
//
//

#include "Layers/Message/SystemMessageLayer.h"

#include "Datas/Message/SystemMessageData.h"

// コンストラクタ
SystemMessageLayer::SystemMessageLayer()
{FUNCLOG}

// デストラクタ
SystemMessageLayer::~SystemMessageLayer()
{FUNCLOG}

// 初期化
bool SystemMessageLayer::init(SystemMessageData *data, function<void()> onCloseCallback)
{
    CC_SAFE_RETAIN(data);
    
    queue<SystemMessageData*> datas {};
    datas.push(data);
    
    return this->init(datas, onCloseCallback);
}

// 初期化
bool SystemMessageLayer::init(const queue<SystemMessageData*>& datas, function<void()> onCloseCallback)
{
	FUNCLOG
    
    this->datas = datas;
    
    Size winSize {Director::getInstance()->getWinSize()};
	
	// 枠を生成
    Sprite* frame { Sprite::create() };
	frame->setTextureRect(Rect(0, 0, winSize.width, winSize.height / 5));
	frame->setColor(Color3B::BLACK);
	frame->setPosition(Point(winSize.width / 2, frame->getContentSize().height / 2));
	this->addChild(frame);
    this->frame = frame;
	
    return MessageLayer::init(onCloseCallback);
}

// メッセージを生成
Label* SystemMessageLayer::createMessage()
{
    Label* message { Label::createWithTTF(this->datas.front()->getMessage(), "fonts/cinecaption2.28.ttf", 24.f) };
    message->setHorizontalAlignment(TextHAlignment::CENTER);
    message->setVerticalAlignment(TextVAlignment::CENTER);
    message->setPosition(this->frame->getContentSize() / 2);
    this->frame->addChild(message);
    
    CC_SAFE_RELEASE(this->datas.front());
    this->datas.pop();
    
    return message;
}

// 次のページがあるか
bool SystemMessageLayer::hasNextPage()
{
    return !this->datas.empty();
}