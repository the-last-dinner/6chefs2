//
//  EventObject.cpp
//  LastSupper
//
//  Created by Kohei on 2015/07/05.
//
//

#include "MapObjects/EventObject.h"

// コンストラクタ
EventObject::EventObject()
{FUNCLOG}

// デストラクタ
EventObject::~EventObject()
{FUNCLOG}

// create関数
EventObject* EventObject::create()
{
	EventObject* pEventObj = new(nothrow)EventObject();
	if (pEventObj && pEventObj->init())
	{
		// オブジェクトを自動メモリ管理へ登録
		pEventObj->autorelease();
		return pEventObj;
	}
	CC_SAFE_DELETE(pEventObj);
	return nullptr;
}

// 初期化
bool EventObject::init()
{
	if(!Node::init()) return false;
    
    this->setCollisionRect(Rect(0, 0, this->getContentSize().width, this->getContentSize().height));
	
	return true;
}
