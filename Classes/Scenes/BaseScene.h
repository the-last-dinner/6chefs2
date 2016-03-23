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

class ConfigEventListenerLayer;
class LoadingLayer;
class SceneData;

class BaseScene : public Scene
{
// インスタンス変数
protected:
    SceneData* data { nullptr };
    ConfigEventListenerLayer* configListener { nullptr };
    bool preloaded { false };
    
// インスタンスメソッド
private:
	virtual void onPreloadFinished(LoadingLayer* loadingLayer) = 0;
protected:
	BaseScene();
	virtual ~BaseScene();
	bool init(SceneData* data);
    virtual void onEnter() override;
};

#endif // __BASE_SCENE_H__
