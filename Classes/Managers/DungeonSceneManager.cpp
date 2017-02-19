//
//  DungeonSceneManager.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/04.
//
//

#include "Managers/DungeonSceneManager.h"

#include "Datas/Scene/DungeonSceneData.h"
#include "Datas/MapObject/EnemyData.h"

#include "Event/EventFactory.h"
#include "Event/GameEventHelper.h"
#include "Event/EventScript.h"

#include "Layers/Dungeon/TiledMapLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/Character.h"
#include "MapObjects/Party.h"
#include "MapObjects/Status/Stamina.h"

#include "Models/CommonEventScripts.h"
#include "Models/EquipItemEvent.h"
#include "Models/StopWatch.h"

#include "Scenes/DungeonScene.h"
#include "Scenes/DungeonCameraScene.h"

#include "Tasks/CameraTask.h"
#include "Tasks/EnemyTask.h"
#include "Tasks/EventTask.h"
#include "Tasks/PlayerControlTask.h"


#include "UI/StaminaBar.h"

// 唯一のインスタンス
static DungeonSceneManager* _instance {nullptr};

// インスタンスを取得
DungeonSceneManager* DungeonSceneManager::getInstance()
{
    if(!_instance) _instance = new DungeonSceneManager();
    
    return _instance;
}

// インスタンスの破棄
void DungeonSceneManager::destroy()
{
    delete _instance;
    _instance = nullptr;
}

// コンストラクタ
DungeonSceneManager::DungeonSceneManager()
{
    FUNCLOG

    // イベントファクトリを生成
    EventFactory* eventFactory {EventFactory::create()};
    CC_SAFE_RETAIN(eventFactory);
    this->eventFactory = eventFactory;
    
    // イベントバリデータを生成
    GameEventHelper* gameEventHelper {GameEventHelper::create()};
    CC_SAFE_RETAIN(gameEventHelper);
    this->gameEventHelper = gameEventHelper;
    
    // スタミナ生成
    Stamina* stamina {Stamina::create()};
    CC_SAFE_RETAIN(stamina);
    this->stamina = stamina;
    
    // 共通イベントスクリプト生成
    CommonEventScripts* commonEventScripts {CommonEventScripts::create()};
    commonEventScripts->loadEventScripts(PlayerDataManager::getInstance()->getLocalData()->getChapterId());
    CC_SAFE_RETAIN(commonEventScripts);
    this->commonEventScripts = commonEventScripts;
    
    // 装備イベント生成
    EquipItemEvent* equipItemEvent {EquipItemEvent::create(commonEventScripts)};
    equipItemEvent->setEquipmentCache(
        PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(DirectionRight()),
        PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(DirectionLeft())
    );
    CC_SAFE_RETAIN(equipItemEvent);
    this->equipItemEvent = equipItemEvent;
}

// デストラクタ
DungeonSceneManager::~DungeonSceneManager()
{
    FUNCLOG

    CC_SAFE_RELEASE_NULL(this->eventFactory);
    CC_SAFE_RELEASE_NULL(this->gameEventHelper);
    CC_SAFE_RELEASE_NULL(this->stamina);
    CC_SAFE_RELEASE_NULL(this->commonEventScripts);
    CC_SAFE_RELEASE_NULL(this->equipItemEvent);
};

#pragma mark -
#pragma mark Getter Methods

// シーンを取得
DungeonScene* DungeonSceneManager::getScene() const { return dynamic_cast<DungeonScene*>(Director::getInstance()->getRunningScene()); }

// マップレイヤを取得
TiledMapLayer* DungeonSceneManager::getMapLayer() const { return this->getScene()->_mapLayer; }

// オブジェクトリストを取得
MapObjectList* DungeonSceneManager::getMapObjectList() const { return this->getMapLayer()->getMapObjectList(); }

// イベントファクトリを取得
EventFactory* DungeonSceneManager::getEventFactory() const { return this->eventFactory; }

// イベントスクリプトを取得
EventScript* DungeonSceneManager::getEventScript() const { return this->getEventTask()->getEventScript(); }

// 共通イベントスクリプトを取得
CommonEventScripts* DungeonSceneManager::getCommonEventScriptsObject() { return this->commonEventScripts; }

// スクリプトバリデータを取得
GameEventHelper* DungeonSceneManager::getGameEventHelper() const { return this->gameEventHelper; }

// パーティを取得
Party* DungeonSceneManager::getParty() { return this->getScene()->_party; }

// 環境光レイヤを取得
AmbientLightLayer* DungeonSceneManager::getAmbientLayer() const { return this->getScene()->_ambientLightLayer; };

// イベントタスクを取得
EventTask* DungeonSceneManager::getEventTask() const { return this->getScene()->_eventTask; }

// カメラを取得
CameraTask* DungeonSceneManager::getCamera() const { return this->getScene()->_cameraTask; };

// アイテム装備イベントを取得
EquipItemEvent* DungeonSceneManager::getEquipItemEvent() const { return this->equipItemEvent; };

#pragma mark -
#pragma mark Scene

// フェードアウト
void DungeonSceneManager::fadeOut(const Color3B& color, const float duration, function<void()> callback)
{
    // 既にフェードアウトしている場合は無視
    if(this->cover)
    {
        callback();
        return;
    }
    
    Sprite* cover { Sprite::create() };
    cover->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    cover->setColor(color);
    cover->setPosition(cover->getContentSize() / 2);
    this->getScene()->addChild(cover, Priority::SCREEN_COVER);
    CC_SAFE_RETAIN(cover);
    this->cover = cover;
    
    cover->setOpacity(0.f);
    this->getScene()->runAction(Sequence::createWithTwoActions(TargetedAction::create(cover, FadeIn::create(duration)), CallFunc::create(callback)));
}

// フェードイン
void DungeonSceneManager::fadeIn(const float duration, function<void()> callback)
{
    if(!this->cover)
    {
        callback();
        return;
    }
    
    this->cover->runAction(
        Sequence::create(
            FadeOut::create(duration),
            CallFunc::create(callback),
            CallFunc::create([this](){CC_SAFE_RELEASE_NULL(this->cover);}),
            nullptr
        )
    );
}

// フェード用カバーを取得
Sprite* DungeonSceneManager::getCover() const
{
    return this->cover;
}

// ロケーションを取得
Location DungeonSceneManager::getLocation() const
{
    return this->getScene()->getData()->getInitialLocation();
}

#pragma mark -
#pragma mark TiledMapLayer

// マップの大きさを取得
Size DungeonSceneManager::getMapSize() const
{
    return this->getMapLayer()->getMapSize();
}

// マップオブジェクトをマップに追加
void DungeonSceneManager::addMapObject(MapObject* mapObject)
{
    this->getMapLayer()->addMapObject(mapObject);
}

// 敵をマップに配置
void DungeonSceneManager::addEnemy(Enemy* enemy)
{
    this->getMapLayer()->addEnemy(enemy);
    
    // スタミナバー表示
    this->getScene()->_staminaBar->slideIn();
}

// マップオブジェクトの位置を設定
void DungeonSceneManager::setMapObjectPosition(MapObject *mapObject)
{
    this->getMapLayer()->setMapObjectPosition(mapObject);
}

#pragma mark -
#pragma mark Director

// ダンジョンから抜ける
void DungeonSceneManager::exitDungeon(Scene* scene)
{
    this->getScene()->onExitDungeon();
    Director::getInstance()->replaceScene(scene);
}

// マップ切り替え
void DungeonSceneManager::changeMap(const Location& destLocation, const Location& currentLocation, const int initEventId)
{
    // 使っていないテクスチャをアンキャッシュ
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    
    // 敵を止める
    this->getScene()->_enemyTask->stop();
    
    // 敵情報を生成し直して格納
    vector<SummonData> summonDatas { this->getScene()->_enemyTask->createDatas(this->getMapObjectList()->getEnemiesAll(), destLocation, currentLocation, this->getLocation()) };
    this->summonDatas.clear();
    this->summonDatas = summonDatas;
    
    // 敵データが空になっていたら
    if(summonDatas.empty()) this->getScene()->onAllEnemyRemoved();
    
    // 主人公一行の位置を登録
    vector<CharacterData> members { this->getParty()->getMembersData() };
    int memberCount = members.size();
    for(int i=0; i < memberCount; i++)
    {
        members[i].location = destLocation;
    }
    PlayerDataManager::getInstance()->getLocalData()->setLocation(members);
    
    // 必要な情報を設定していく
    DungeonSceneData* data { DungeonSceneData::create(destLocation) };
    data->setInitialEventId(initEventId);
    
    // 現在入力されている方向キーからリスナ生成
    EventListenerKeyboardLayer* listener {EventListenerKeyboardLayer::create(this->getPressedCursorKeys(), this->isPressed(Key::DASH))};
    
    DungeonScene* scene {DungeonScene::create(data, listener)};
    
    Director::getInstance()->replaceScene(scene);
}

// カメラシーンへ切り替え（スタックに積む）
void DungeonSceneManager::pushCameraScene(DungeonCameraScene* scene)
{
    this->getScene()->_listener->releaseKeyAll();
    Director::getInstance()->pushScene(scene);
}

// カメラシーンを終了する
void DungeonSceneManager::popCameraScene()
{
    Director::getInstance()->popScene();
}

#pragma mark -
#pragma mark EventListener

// 指定キーが押されているかチェック
bool DungeonSceneManager::isPressed(const Key& key)
{
    return this->getScene()->_listener->isPressed(key);
}

// 入力されている方向キーを取得
vector<Key> DungeonSceneManager::getPressedCursorKeys() const
{
    return this->getScene()->_listener->getPressedCursorKeys();
}

#pragma mark -
#pragma mark EnemyTask

// 敵の出現情報を取得
vector<SummonData> DungeonSceneManager::getSummonDatas() const
{
    return this->summonDatas;
}

// 敵を削除
void DungeonSceneManager::removeEnemy(const int enemyId)
{
    this->getScene()->_enemyTask->removeEnemy(enemyId);
}

void DungeonSceneManager::removeEnemyByObjectId(const int objectId)
{
    this->getScene()->_enemyTask->removeEnemyByObjectId(objectId);
}

// 敵が存在するか確認
bool DungeonSceneManager::existsEnemy() const
{
    return this->getScene()->_enemyTask->existsEnemy();
}

#pragma mark -
#pragma mark EventTask

// イベントを実行
void DungeonSceneManager::runEvent(const int eventId)
{
    this->getEventTask()->runEvent(eventId);
}

// イベント実行
void DungeonSceneManager::runEvent(const vector<int>& eventIds)
{
    this->getEventTask()->runEvent(eventIds);
}

// 非同期イベント実行
void DungeonSceneManager::runEventAsync(GameEvent* event)
{
    this->getEventTask()->runEventAsync(event);
}

// キューにイベントを後ろから詰める
void DungeonSceneManager::pushEventBack(const int eventId)
{
    this->getEventTask()->pushEventBack(eventId);
}

// キューにイベントを前から詰める
void DungeonSceneManager::pushEventFront(const int eventId)
{
    this->getEventTask()->pushEventFront(eventId);
}

// キューにイベントを後ろから詰める
void DungeonSceneManager::pushEventBack(GameEvent* event)
{
    this->getEventTask()->pushEventBack(event);
}

// キューにイベントを前から詰める
void DungeonSceneManager::pushEventFront(GameEvent* event)
{
    this->getEventTask()->pushEventFront(event);
}

// キューにあるイベントを実行
void DungeonSceneManager::runEventQueue()
{
    this->getEventTask()->runEventQueue();
}

// キューにイベントがあるか
bool DungeonSceneManager::existsEvent() const
{
    return this->getEventTask()->existsEvent();
}

// 実行しているイベントのIDを取得
int DungeonSceneManager::getRunningEventId() const
{
    return this->getEventTask()->getRunningEventId();
}

// 実行しているイベントを取得
GameEvent* DungeonSceneManager::getRunningEvent() const
{
    return this->getEventTask()->getRunningEvent();
}

// イベント実行中か
bool DungeonSceneManager::isEventRunning() const
{
    return this->getEventTask()->isEventRunning();
}

#pragma mark -
#pragma mark StopWatch

// ストップウォッチインスタンスの取得
StopWatch* DungeonSceneManager::getStopWatch()
{
    if(!this->stopWatch)
    {
        this->stopWatch = StopWatch::create(0);
        CC_SAFE_RETAIN(this->stopWatch);
    }
    return this->stopWatch;
}

// ストップウォッチをリリース
void DungeonSceneManager::releaseStopWatch()
{
    CC_SAFE_RELEASE_NULL(this->stopWatch);
}

// ストップォッチを一時停止
void DungeonSceneManager::pauseStopWatch()
{
    if(this->stopWatch)
    {
        this->stopWatch->stopCountDown();
    }
}

// カウントダウンをスタート
void DungeonSceneManager::startStopWatch()
{
    if(this->stopWatch)
    {
        this->stopWatch->startCountDown();
    }
}

// ストップウォッチインスタンスが存在するかどうか
bool DungeonSceneManager::existsStopWatch()
{
    return this->stopWatch;
}

#pragma mark -
#pragma mark Stamina

Stamina* DungeonSceneManager::getStamina() const
{
    return this->stamina;
}

void DungeonSceneManager::setStaminaCallback(function<void (float)> callback)
{
    this->stamina->onPercentageChanged = callback;
}
