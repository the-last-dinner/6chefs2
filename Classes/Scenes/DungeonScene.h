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
class DungeonSceneEventHandler;

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
class CountDownDisplay;
class EventListenerKeyboardLayer;

class Battle;

class DungeonScene : public BaseScene
{
// クラスメソッド
public:
	CREATE_FUNC_WITH_PARAM(DungeonScene, DungeonSceneData*);
    CREATE_FUNC_WITH_TWO_PARAM(DungeonScene, DungeonSceneData*, EventListenerKeyboardLayer*);
	
// インスタンス変数
protected:
    DungeonSceneEventHandler* _handler { nullptr };
    EventListenerKeyboardLayer* _listener { nullptr };
    TiledMapLayer* _mapLayer { nullptr };
    AmbientLightLayer* _ambientLightLayer { nullptr };
    FocusLightLayer* _focusLightLayer { nullptr };
    
    CameraTask* _cameraTask { nullptr };
    EnemyTask* _enemyTask { nullptr };
    EventTask* _eventTask { nullptr };
    PlayerControlTask* _playerControlTask { nullptr };
    
    LoadingLayer* _loadingLayer { nullptr };
    
    Party* _party { nullptr };
    StaminaBar* _staminaBar { nullptr };
    CountDownDisplay* _countDownDisplay { nullptr };
    
// インスタンスメソッド
protected:
    DungeonScene();
    virtual ~DungeonScene();
    
protected:
	virtual bool init(DungeonSceneData* data);
    virtual bool init(DungeonSceneData* data, EventListenerKeyboardLayer* listener);
    
public:
// コールバック
    virtual void onEnter() override;
	virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    virtual void onInitEventFinished(LoadingLayer* loadingLayer);
    virtual void onAfterInitEventFinished();
    virtual void onPassedEventFinished();
    virtual void onMenuKeyPressed();
    void onPopMenuScene();
    void onBackToTitleSelected();
    void onAllEnemyRemoved();
    void onExitDungeon();
    void onEventStart();
    void onEventFinished();
    
    DungeonSceneData* getData() const;
    
    friend class DungeonSceneManager;
    friend class DungeonSceneEventHandler;
    
public:
    virtual void onEnterPushedScene() override;
    virtual void onExitPushedScene() override;
    void onBattleStart(Battle* battle);
    void onBattleFinished(Battle* battle);
    CountDownDisplay* getCountDownDisplay();
};


#endif // __DUNGEON_SCENE_H__
