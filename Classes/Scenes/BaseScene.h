//
//  BaseScene.h
//  LastSupper
//
//  Created by Kohei on 2015/06/05.
//
//

#ifndef __BASE_SCENE_H__
#define __BASE_SCENE_H__

#include "Common.h"

class EventListenerKeyboardLayer;
class LoadingLayer;
class SceneData;

class BaseScene : public Scene
{
// インスタンスメソッド
private:
	virtual void onPreloadFinished(LoadingLayer* loadingLayer) = 0;
protected:
	BaseScene();
	~BaseScene();
	bool init(SceneData* data);
    virtual void onEnter() override;
	
// インスタンス変数
protected:
	SceneData* data { nullptr };
    bool preloaded { false };
};

#endif // __BASE_SCENE_H__
