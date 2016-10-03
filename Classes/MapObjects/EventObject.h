//
//  EventObject.h
//  LastSupper
//
//  Created by Kohei on 2015/07/05.
//
//

#ifndef __EVENT_OBJECT_H__
#define __EVENT_OBJECT_H__

#include "MapObject.h"

class EventObject : public MapObject
{
// クラスメソッド
public:
	static EventObject* create();
	
// インスタンスメソッド
private:
	EventObject();
	~EventObject();
	virtual bool init();
};

#endif // __EVENT_OBJECT_H__
