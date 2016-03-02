//
//  StoryMessageLayer.h
//  LastSupper
//
//  Created by Kohei on 2015/08/02.
//
//

#ifndef __STORY_MESSAGE_LAYER_H__
#define __STORY_MESSAGE_LAYER_H__

#include "Layers/Message/MessageLayer.h"

class StoryMessageData;

class StoryMessageLayer : public MessageLayer
{
// クラスメソッド
public:
    static StoryMessageLayer* create(const string& title, const queue<StoryMessageData*>& datas, function<void()> onCloseCallback);

// インスタンス変数
private:
    queue<StoryMessageData*> datas {};
    
// インスタンスメソッド
private:
	StoryMessageLayer();
	~StoryMessageLayer();
	virtual bool init(const string& title, const queue<StoryMessageData*>& datas, function<void()> onCloseCallback);
    virtual Label* createMessage();
    virtual bool hasNextPage();
};

#endif // __STORY_MESSAGE_LAYER_H__
