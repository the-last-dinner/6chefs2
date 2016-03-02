//
//  SystemMessageLayer.h
//  LastSupper
//
//  Created by Kohei on 2015/07/12.
//
//

#ifndef __SYSTEM_MESSAGE_LAYER_H__
#define __SYSTEM_MESSAGE_LAYER_H__

#include "Layers/Message/MessageLayer.h"

class SystemMessageData;

class SystemMessageLayer : public MessageLayer
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(SystemMessageLayer, SystemMessageData*, function<void()>);
	CREATE_FUNC_WITH_TWO_PARAM(SystemMessageLayer, const queue<SystemMessageData*>&, function<void()>);

// インスタンス変数
private:
    queue<SystemMessageData*> datas {};
    
// インスタンスメソッド
private:
	SystemMessageLayer();
	~SystemMessageLayer();
    virtual bool init(SystemMessageData* data, function<void()> onCloseCallback);
	virtual bool init(const queue<SystemMessageData*>& datas, function<void()> onCloseCallback);
    virtual Label* createMessage();
    virtual bool hasNextPage();
};

#endif // __SYSTEM_MESSAGE_LAYER_H__
