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
    SceneData* _data { nullptr };
    ConfigEventListenerLayer* _configListener { nullptr };
    bool _isPreloaded { false };
    
// インスタンスメソッド
private:
	virtual void onPreloadFinished(LoadingLayer* loadingLayer) = 0;
protected:
	BaseScene();
	virtual ~BaseScene();
	bool init(SceneData* data);
    virtual void onEnter() override;
    virtual LoadingLayer* createLoadingLayer();
public:
    virtual void onEnterPushedScene(){};
    virtual void onExitPushedScene(){};
};

#endif // __BASE_SCENE_H__
