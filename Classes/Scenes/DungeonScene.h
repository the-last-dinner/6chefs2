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
class EventListenerKeyboardLayer;

class DungeonScene : public BaseScene
{
// クラスメソッド
public:
	CREATE_FUNC_WITH_PARAM(DungeonScene, DungeonSceneData*);
    CREATE_FUNC_WITH_TWO_PARAM(DungeonScene, DungeonSceneData*, EventListenerKeyboardLayer*)
	
// インスタンス変数
protected:
    EventListenerKeyboardLayer* listener { nullptr };
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
    virtual bool init(DungeonSceneData* data, EventListenerKeyboardLayer* listener);
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
};


#endif // __DUNGEON_SCENE_H__
