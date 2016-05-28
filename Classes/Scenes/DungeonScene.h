//
//  DungeonScene.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/06/14.
//
//

#ifndef __DUNGEON_SCENE_H__
#define __DUNGEON_SCENE_H__

#include "Scenes/BaseScene.h"

class DungeonSceneData;

class AmbientLightLayer;
class TiledMapLayer;
class FocusLightLayer;

class CameraTask;
class EnemyTask;
class EventTask;
class PlayerControlTask;

class LoadingLayer;
class Party;
class StaminaBar;

class DungeonScene : public BaseScene
{
// クラスメソッド
public:
	CREATE_FUNC_WITH_PARAM(DungeonScene, DungeonSceneData*);
	
// インスタンス変数
protected:
    TiledMapLayer* mapLayer { nullptr };
    AmbientLightLayer* ambientLightLayer { nullptr };
    FocusLightLayer* focusLightLayer { nullptr };
    
    CameraTask* cameraTask { nullptr };
    EnemyTask* enemyTask { nullptr };
    EventTask* eventTask { nullptr };
    PlayerControlTask* playerControlTask { nullptr };
    
    LoadingLayer* loadingLayer { nullptr };
    
    Party* party { nullptr };
    StaminaBar* staminaBar { nullptr };
    
// インスタンスメソッド
protected:
    DungeonScene();
    virtual ~DungeonScene();
    
protected:
	virtual bool init(DungeonSceneData* data);
    virtual void onEnter() override;
	virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    virtual void onInitEventFinished(LoadingLayer* loadingLayer);
    virtual void onAfterInitEventFinished();
    virtual void onPassedEventFinished();
    virtual void onMenuKeyPressed();
    void onPopMenuScene();
    void onBackToTitleSelected();
    void onContactWithEnemy();
    void onAllEnemyRemoved();
    void onExitDungeon();
    void setLight();
    void onEventStart();
    void onEventFinished();
    
    DungeonSceneData* getData() const;
    
    friend class DungeonSceneManager;
    friend class CameraTask;
    friend class EnemyTask;
    friend class EventTask;
    friend class PlayerControlTask;
};


#endif // __DUNGEON_SCENE_H__
