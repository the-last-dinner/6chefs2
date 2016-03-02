//
//  DungeonScene.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/06/14.
//
//

#include "Scenes/DungeonScene.h"

#include "Datas/Scene/DungeonSceneData.h"

#include "Effects/AmbientLightLayer.h"

#include "Event/EventScript.h"

#include "Layers/Dungeon/TiledMapLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "Layers/LoadingLayer.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/Character.h"
#include "MapObjects/Party.h"

#include "Managers/DungeonSceneManager.h"
#include "Managers/NotificationManager.h"

#include "Models/Stamina.h"

#include "Scenes/DungeonMenuScene.h"
#include "Scenes/GameOverScene.h"
#include "Scenes/TitleScene.h"

#include "Tasks/EnemyTask.h"
#include "Tasks/CameraTask.h"
#include "Tasks/EventTask.h"
#include "Tasks/PlayerControlTask.h"

#include "UI/StaminaBar.h"

// コンストラクタ
DungeonScene::DungeonScene() {FUNCLOG}

// デストラクタ
DungeonScene::~DungeonScene()
{
    FUNCLOG

    CC_SAFE_RELEASE_NULL(this->party);
}

// 初期化
bool DungeonScene::init(DungeonSceneData* data)
{
    if(!BaseScene::init(data)) return false;
    
    // イベントリスナ生成
    EventListenerKeyboardLayer* listener { EventListenerKeyboardLayer::create() };
    this->addChild(listener);
    this->listener = listener;
    
    return true;
}

// 初期化
bool DungeonScene::init(DungeonSceneData* data, EventListenerKeyboardLayer* listener)
{
    if(!BaseScene::init(data)) return false;
    
    this->addChild(listener);
    this->listener = listener;
    
    return true;
}

// シーン切り替え終了時
void DungeonScene::onEnter()
{
    BaseScene::onEnter();
    
    // フェード用カバー
    Sprite* cover {DungeonSceneManager::getInstance()->getCover()};
    
    if(!cover) return;
    
    cover->removeFromParent();
    this->addChild(cover);
}

// リソースプリロード完了時の処理
void DungeonScene::onPreloadFinished(LoadingLayer* loadingLayer)
{
    // スタミナ減少状態を解除
    DungeonSceneManager::getInstance()->getStamina()->setDecreasing(false);
    
	// マップレイヤーを生成
	TiledMapLayer* mapLayer {TiledMapLayer::create(this->getData()->getInitialLocation())};
    mapLayer->setLocalZOrder(Priority::MAP);
	this->addChild(mapLayer);
	this->mapLayer = mapLayer;
    
    // 環境光レイヤー生成
    AmbientLightLayer* ambientLightLayer {AmbientLightLayer::create(AmbientLightLayer::ROOM)};
    ambientLightLayer->setLocalZOrder(Priority::AMBIENT_LIGHT);
    this->addChild(ambientLightLayer);
    this->ambientLightLayer = ambientLightLayer;
    
    // 敵処理クラス生成
    EnemyTask* enemyTask { EnemyTask::create() };
    this->addChild(enemyTask);
    this->enemyTask = enemyTask;
    
    // カメラ処理クラス生成
    CameraTask* cameraTask {CameraTask::create()};
    this->addChild(cameraTask);
    this->cameraTask = cameraTask;
    
    // イベント処理クラス生成
    EventTask* eventTask { EventTask::create() };
    this->addChild(eventTask);
    this->eventTask = eventTask;
    
    // プレイヤー操作処理クラス生成
    PlayerControlTask* playerControlTask {PlayerControlTask::create()};
    this->addChild(playerControlTask);
    this->playerControlTask = playerControlTask;
    
    // 主人公一行を生成
    Party* party { Party::create(PlayerDataManager::getInstance()->getLocalData()->getPartyMemberAll()) };
    CC_SAFE_RETAIN(party);
    this->party = party;
    
    // 主人公一行をマップに配置
    mapLayer->setParty(party);
    
    // スタミナバー生成
    StaminaBar* staminaBar { StaminaBar::create() };
    staminaBar->setLocalZOrder(Priority::STAMINA_BAR);
    this->addChild(staminaBar);
    this->staminaBar = staminaBar;
    
    // スタミナが変化した際のコールバック指定
    DungeonSceneManager::getInstance()->setStaminaCallback(CC_CALLBACK_1(StaminaBar::setPercentage, staminaBar));
    
    // スタミナ残量を反映
    staminaBar->setPercentage(DungeonSceneManager::getInstance()->getStamina()->getPercentage());
    
    // 敵が存在すれば、スタミナバーを表示しておく
    if(enemyTask->existsEnemy()) staminaBar->slideIn();
    
    // イベント処理クラスにコールバック設定
    eventTask->onRunEvent = CC_CALLBACK_0(DungeonScene::onRunEvent, this);
    eventTask->onAllEventFinished = CC_CALLBACK_0(DungeonScene::onAllEventFinished, this);
    
    // 敵処理クラスにコールバック設定
    enemyTask->onAllEnemyRemoved = CC_CALLBACK_0(DungeonScene::onAllEnemyRemoved, this);
    
    // オブジェクトリストにコールバック設定
    mapLayer->getMapObjectList()->onContactWithEnemy = CC_CALLBACK_0(DungeonScene::onContactWithEnemy, this);
    
    // リスナにコールバック設定
    this->listener->onCursorKeyPressed = [playerControlTask, party](const Key& key){playerControlTask->turn(key, party);};
    this->listener->onSpaceKeyPressed = [playerControlTask, party]{playerControlTask->search(party);};
    this->listener->onMenuKeyPressed = CC_CALLBACK_0(DungeonScene::onMenuKeyPressed, this);
    
    // Trigger::INITを実行
    eventTask->runEvent(mapLayer->getMapObjectList()->getEventIds(Trigger::INIT), [this, loadingLayer](){this->onInitEventFinished(loadingLayer);});
}

// Trigger::INITのイベント実行後
void DungeonScene::onInitEventFinished(LoadingLayer* loadingLayer)
{
    this->setLight();
    cameraTask->setTarget( this->party->getMainCharacter() );
    
    this->enemyTask->start(this->getData()->getInitialLocation().map_id);
    
    // ローディング終了
    loadingLayer->onLoadFinished();
    
    // Trigger::AFTER_INITを実行
    this->eventTask->runEvent(this->mapLayer->getMapObjectList()->getEventIds(Trigger::AFTER_INIT), CC_CALLBACK_0(DungeonScene::onAfterInitEventFinished, this));
}

// Trigger::AFTER_INITのイベント終了時
void DungeonScene::onAfterInitEventFinished()
{
    this->eventTask->runEvent({this->getData()->getInitialEventId()}, CC_CALLBACK_0(DungeonScene::onPassedEventFinished, this));
}

// マップ移動前に渡されたイベント終了時
void DungeonScene::onPassedEventFinished()
{
    // マップ名通知
    NotificationManager::getInstance()->notifyMapName(this->getData()->getInitialLocation().map_id);
    
    this->playerControlTask->setControlEnable(true, this->party);
}

// メニューキー押したとき
void DungeonScene::onMenuKeyPressed()
{
    // イベント中なら無視
    if(this->eventTask->isEventRunning() || this->eventTask->existsEvent()) return;
    
    // 敵が存在すれば無視
    if(this->enemyTask->existsEnemy()) return;
    
    // リスナーを停止
    this->listener->setEnabled(false);
    
    // カウントダウンしてれば停止
    DungeonSceneManager::getInstance()->pauseStopWatch();
    
    // パーティの位置をセット
    PlayerDataManager::getInstance()->getLocalData()->setLocation(DungeonSceneManager::getInstance()->getParty()->getMembersData());
    
    // スクショをとって、ダンジョンメニューシーンをプッシュ
    string path = LastSupper::StringUtils::strReplace((string)"global" + SAVE_EXTENSION, "screen0.png", FileUtils::getInstance()->fullPathForFilename((string)"save/global" + SAVE_EXTENSION));
    utils::captureScreen([=](bool success, string filename){
     if(success)
     {
         Sprite* screen = Sprite::create(filename);
         DungeonMenuScene* menu = DungeonMenuScene::create(screen->getTexture());
         menu->onBackToTitleSelected = CC_CALLBACK_0(DungeonScene::onBackToTitleSelected, this);
         menu->onPopMenuScene = CC_CALLBACK_0(DungeonScene::onPopMenuScene, this);
         
         // メニューシーンをプッシュ
         Director::getInstance()->pushScene(menu);
         // cache削除
         Director::getInstance()->getTextureCache()->removeTextureForKey(filename);
     }
    }, path);
}

// メニューシーンから戻ってきた時
void DungeonScene::onPopMenuScene()
{
    // カウントダウンをしれてば再開
    DungeonSceneManager::getInstance()->startStopWatch();
    
    // たいまつを装備していればライトをつける
    this->setLight();
    
    // 操作可能に戻す
    this->listener->setEnabled(true);
}

// メニューシーンでタイトルへ戻るを選択した時
void DungeonScene::onBackToTitleSelected()
{
    Director::getInstance()->popScene();
    this->onExitDungeon();
    Director::getInstance()->replaceScene(TitleScene::create());
}

// 主人公が敵に触れた時
void DungeonScene::onContactWithEnemy()
{
    if (DebugManager::getInstance()->isInvincibleMode()) return;
    this->onExitDungeon();
    Director::getInstance()->replaceScene(GameOverScene::create(GameOverScene::Type::BLOOD));
}

// 敵が全ていなくなった時
void DungeonScene::onAllEnemyRemoved()
{
    // スタミナバーを隠す
    this->staminaBar->slideOut();
    
    // 主人公疲労のBGMを消す
    if(SoundManager::getInstance()->isPlaying(Resource::BGM::TIRED)) SoundManager::getInstance()->stopBGM(Resource::BGM::TIRED);
}

// ダンジョンシーンから他のシーンへ移動する時
void DungeonScene::onExitDungeon()
{
    DungeonSceneManager::destroy();
    SoundManager::getInstance()->stopBGMAll();
}

// ライトの管理
void DungeonScene::setLight()
{
    if (PlayerDataManager::getInstance()->getLocalData()->isEquipedItem(1514))
    {
        this->party->getMainCharacter()->setLight(Light::create(Light::Information(50)), this->ambientLightLayer);
    }
    else
    {
        this->party->getMainCharacter()->removeLight();
    }
}

// イベントを実行する時
void DungeonScene::onRunEvent()
{
    // プレイヤーの操作を無効に
    this->playerControlTask->setControlEnable(false, party);
    
    // 全てのオブジェクトの動きを止める
    this->mapLayer->getMapObjectList()->moveStopAllObjects();
    
    // スタミナの増減を一時停止
    DungeonSceneManager::getInstance()->getStamina()->setPaused(true);
}

// イベントキューが空になった時
void DungeonScene::onAllEventFinished()
{
    // プレイヤーの操作を有効に
    this->playerControlTask->setControlEnable(true, party);
    
    // 全てのオブジェクトの自動移動を開始する
    this->mapLayer->getMapObjectList()->moveStartAllObjects();
    
    // スタミナの増減を再開
    DungeonSceneManager::getInstance()->getStamina()->setPaused(false);
}

// データクラスを取得
DungeonSceneData* DungeonScene::getData() const { return dynamic_cast<DungeonSceneData*>(this->data); }
