//
//  DungeonSceneManager.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/04.
//
//

#ifndef __LastSupper__DungeonSceneManager__
#define __LastSupper__DungeonSceneManager__

#include "Common.h"

#include "Datas/MapObject/EnemyData.h"

class AmbientLightLayer;
class CommonEventScripts;
class DungeonCameraScene;
class DungeonScene;
class TiledMapLayer;
class MapObject;
class MapObjectList;
class EventFactory;
class EventScript;
class GameEventHelper;
class Party;
class GameEvent;
class Enemy;
class StopWatch;
class Stamina;
class CameraTask;
class EventTask;
class EquipItemEvent;

// ダンジョンシーンのマップ間で共有すべき情報を持つシングルトンクラス
class DungeonSceneManager
{
// クラスメソッド
public:
    static DungeonSceneManager* getInstance();
    static void destroy();
    
// インスタンス変数
private:
    EventFactory* eventFactory { nullptr };
    GameEventHelper* gameEventHelper { nullptr };
    Sprite* cover { nullptr };
    vector<SummonData> summonDatas {};
    StopWatch* stopWatch { nullptr };
    Stamina* stamina { nullptr };
    CommonEventScripts* commonEventScripts { nullptr };
    EquipItemEvent* equipItemEvent { nullptr };
    
public:
    bool onReturnFromDungeonMenuScene { false };
    
// インスタンスメソッド
private:
    DungeonSceneManager();                                                  // コンストラクタ
    ~DungeonSceneManager();                                                 // デストラクタ
    DungeonSceneManager(const DungeonSceneManager& other) {};               // コピーコンストラクタ
    DungeonSceneManager &operator = (const DungeonSceneManager& other);     // 代入演算子
public:
    DungeonScene* getScene() const;
    TiledMapLayer* getMapLayer() const;
    MapObjectList* getMapObjectList() const;
    EventFactory* getEventFactory() const;
    GameEventHelper* getGameEventHelper() const;
    EventScript* getEventScript() const;
    CommonEventScripts* getCommonEventScriptsObject();
    Party* getParty();
    AmbientLightLayer* getAmbientLayer() const;
    EventTask* getEventTask() const;
    CameraTask* getCamera() const;
    EquipItemEvent* getEquipItemEvent() const;
    
    // Scene
    void fadeOut(const Color3B& color, const float duration, function<void()> callback);
    void fadeIn(const float duration, function<void()> callback);
    Sprite* getCover() const;
    Location getLocation() const;
    
    // TiledMaplayer
    Size getMapSize() const;
    void addMapObject(MapObject* mapobject);
    void addEnemy(Enemy* enemy);
    void setMapObjectPosition(MapObject* mapObject);
    
    // Director
    void exitDungeon(Scene* scene);
    void changeMap(const Location& destLocation, const Location& currentLocation, const int initEventId);
    void pushCameraScene(DungeonCameraScene* scene);
    void popCameraScene();
    
    // EventListener
    bool isPressed(const Key& key);
    vector<Key> getPressedCursorKeys() const;
    
    // EnemyTask
    vector<SummonData> getSummonDatas() const;
    void removeEnemy(const int enemyId);
    void removeEnemyByObjectId(const int objectId);
    bool existsEnemy() const;
    
    // EventTask
    void runEvent(const int eventId);
    void runEvent(const vector<int>& eventIds);
    void runEventAsync(GameEvent* event);
    void pushEventBack(const int eventId);
    void pushEventFront(const int eventId);
    void pushEventBack(GameEvent* event);
    void pushEventFront(GameEvent* event);
    void runEventQueue();
    bool existsEvent() const;
    int getRunningEventId() const;
    GameEvent* getRunningEvent() const;
    bool isEventRunning() const;
    
    // StopWatch
    StopWatch* getStopWatch();
    void releaseStopWatch();
    void pauseStopWatch();
    void startStopWatch();
    bool existsStopWatch();
    
    // Stamina
    Stamina* getStamina() const;
    void setStaminaCallback(function<void(float)> callback);
};

#endif /* defined(__LastSupper__DungeonSceneManager__) */
