#ifndef  __APP_DELEGATE_H__
#define  __APP_DELEGATE_H__

#include "Common.h"

class  AppDelegate : private cocos2d::Application {
	// インスタンスメソッド
public:
	AppDelegate();
	virtual ~AppDelegate();
	virtual bool applicationDidFinishLaunching();
	virtual void applicationDidEnterBackground();
	virtual void applicationWillEnterForeground();
private:
	void init();
};

#endif // __APP_DELEGATE_H__