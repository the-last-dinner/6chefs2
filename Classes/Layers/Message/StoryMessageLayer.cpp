//
//  StoryMessageLayer.cpp
//  LastSupper
//
//  Created by Kohei on 2015/08/02.
//
//

#include "Layers/Message/StoryMessageLayer.h"

#include "Datas/Message/StoryMessageData.h"

// create関数
StoryMessageLayer* StoryMessageLayer::create(const string& title, const queue<StoryMessageData*>& datas, function<void()> onCloseCallback)
{
    StoryMessageLayer* p {new(nothrow) StoryMessageLayer()};
    if(p && p->init(title, datas, onCloseCallback))
    {
        p->autorelease();
        return p;
    }
    else
    {
        delete p;
        p = nullptr;
        return nullptr;
    }
}

// コンストラクタ
StoryMessageLayer::StoryMessageLayer()
{FUNCLOG}

// デストラクタ
StoryMessageLayer::~StoryMessageLayer()
{FUNCLOG}

// 初期化
bool StoryMessageLayer::init(const string& title, const queue<StoryMessageData*>& datas, function<void()> onCloseCallback)
{
	FUNCLOG
    
    this->datas = datas;
    
    Size winSize {Director::getInstance()->getWinSize()};
	
	// 枠を生成
	Sprite* frame = Sprite::create();
	frame->setTextureRect(Rect(0, 0, winSize.width, winSize.height));
	frame->setColor(Color3B::BLACK);
	frame->setPosition(winSize / 2);
	this->addChild(frame);
    this->frame = frame;
    
    // タイトルを生成
    Label* titleLabel {Label::createWithTTF(title, "fonts/cinecaption2.28.ttf", 36.f)};
    titleLabel->setPosition(WINDOW_WIDTH / 2, winSize.height - titleLabel->getContentSize().height * 4);
    frame->addChild(titleLabel);
	
	return MessageLayer::init(onCloseCallback);
}

// メッセージを生成
Label* StoryMessageLayer::createMessage()
{
    Label* message { Label::createWithTTF(this->datas.front()->getMessage(), "fonts/cinecaption2.28.ttf", 24.f) };
    message->setLineHeight(45.f);
    message->setHorizontalAlignment(TextHAlignment::LEFT);
    message->setVerticalAlignment(TextVAlignment::CENTER);
    message->setPosition(message->getContentSize().width / 2 + WINDOW_WIDTH / 5, this->frame->getContentSize().height / 2);
    this->frame->addChild(message);
    
    CC_SAFE_RELEASE(this->datas.front());
    this->datas.pop();
    
    return message;
}

// 次のページがあるか
bool StoryMessageLayer::hasNextPage()
{
    return !this->datas.empty();
}