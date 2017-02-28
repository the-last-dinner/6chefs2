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
#include "Effects/FocusLightLayer.h"

#include "Event/EventScript.h"

#include "Layers/Dungeon/TiledMapLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "Layers/EventListener/ConfigEventListenerLayer.h"
#include "Layers/LoadingLayer.h"

#include "MapObjects/MapObjectList.h"
#include "MapObjects/Character.h"
#include "MapObjects/Party.h"
#include "MapObjects/Status/Stamina.h"

#include "Managers/DungeonSceneManager.h"
#include "Managers/NotificationManager.h"

#include "Models/EquipItemEvent.h"
#include "Models/StopWatch.h"

#include "Scenes/EventHandler/DungeonSceneEventHandler.h"
#include "Scenes/DungeonMenuScene.h"
#include "Scenes/GameOverScene.h"
#include "Scenes/TitleScene.h"

#include "Tasks/EnemyTask.h"
#include "Tasks/CameraTask.h"
#include "Tasks/EventTask.h"
#include "Tasks/PlayerControlTask.h"

#include "UI/StaminaBar.h"
#include "UI/CountDownDisplay.h"

// コンストラクタ
DungeonScene::DungeonScene() { FUNCLOG }

// デストラクタ
DungeonScene::~DungeonScene()
{
    FUNCLOG

    CC_SAFE_RELEASE_NULL(_party);
    CC_SAFE_RELEASE_NULL(_handler);
}

// 初期化
bool DungeonScene::init(DungeonSceneData* data)
{
    // イベントリスナ生成
    EventListenerKeyboardLayer* listener { EventListenerKeyboardLayer::create() };
    
    return this->init(data, listener);
}

// 初期化
bool DungeonScene::init(DungeonSceneData* data, EventListenerKeyboardLayer* listener)
{
    if (!BaseScene::init(data)) return false;
    
    this->addChild(listener);
    _listener = listener;
    
    _configListener->onOpenkeyconfigMenu = CC_CALLBACK_0(DungeonScene::onEventStart, this);
    _configListener->onKeyconfigMenuClosed = CC_CALLBACK_0(DungeonScene::onEventFinished, this);
    
    // イベントハンドラ生成
    DungeonSceneEventHandler* handler { DungeonSceneEventHandler::create(this) };
    CC_SAFE_RETAIN(handler);
    _handler = handler;
    
    return true;
}

#pragma mark -
#pragma mark Callback

// シーン切り替え終了時
void DungeonScene::onEnter()
{
    BaseScene::onEnter();
    
    if (DungeonSceneManager::getInstance()->onReturnFromDungeonMenuScene) {
        // 装備チェンジイベント実行
        DungeonSceneManager::getInstance()->getEquipItemEvent()->onChangeEquipment(
            PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(DirectionRight()),
            PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(DirectionLeft())
        );
        DungeonSceneManager::getInstance()->onReturnFromDungeonMenuScene = false;
    }
    
    // フェード用カバー
    Sprite* cover { DungeonSceneManager::getInstance()->getCover() };
    
    if (!cover) return;
    
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
	_mapLayer = mapLayer;
    
    // フォーカス光レイヤー生成
    FocusLightLayer* focusLightLayer {FocusLightLayer::create()};
    focusLightLayer->setLocalZOrder(Priority::FOCUS_LIGHT);
    this->addChild(focusLightLayer);
    _focusLightLayer = focusLightLayer;
    
    // 環境光レイヤー生成
    AmbientLightLayer* ambientLightLayer {AmbientLightLayer::create(AmbientLightLayer::DAY)};
    ambientLightLayer->setLocalZOrder(Priority::AMBIENT_LIGHT);
    this->addChild(ambientLightLayer);
    _ambientLightLayer = ambientLightLayer;
    
    // 敵処理クラス生成
    EnemyTask* enemyTask { EnemyTask::create() };
    this->addChild(enemyTask);
    _enemyTask = enemyTask;
    
    // カメラ処理クラス生成
    CameraTask* cameraTask {CameraTask::create()};
    this->addChild(cameraTask);
    _cameraTask = cameraTask;
    
    // イベント処理クラス生成
    EventTask* eventTask { EventTask::create() };
    this->addChild(eventTask);
    _eventTask = eventTask;
    
    // プレイヤー操作処理クラス生成
    PlayerControlTask* playerControlTask {PlayerControlTask::create()};
    this->addChild(playerControlTask);
    _playerControlTask = playerControlTask;
    
    // 主人公一行を生成
    Party* party { Party::create(PlayerDataManager::getInstance()->getLocalData()->getPartyMemberAll()) };
    CC_SAFE_RETAIN(party);
    _party = party;
    
    // 主人公一行をマップに配置
    mapLayer->setParty(party);
    
    // 主人公にフォーカス光を当てる
    focusLightLayer->addTarget(party->getMainCharacter());
    
    // 装備イベントキャッシュの更新
    DungeonSceneManager::getInstance()->getEquipItemEvent()->setEquipmentCache(
        PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(DirectionRight()),
        PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(DirectionLeft())
    );
    
    // スタミナバー生成
    StaminaBar* staminaBar { StaminaBar::create() };
    staminaBar->setLocalZOrder(Priority::STAMINA_BAR);
    this->addChild(staminaBar);
    _staminaBar = staminaBar;
    
    
    // スタミナが変化した際のコールバック指定
    DungeonSceneManager::getInstance()->setStaminaCallback(CC_CALLBACK_1(StaminaBar::setPercentage, _staminaBar));
    
    // スタミナ残量を反映
    _staminaBar->setPercentage(DungeonSceneManager::getInstance()->getStamina()->getPercentage());
    
    // 敵が存在すれば、スタミナバーを表示しておく
    if(enemyTask->existsEnemy()) staminaBar->slideIn();
    
    // カウントダウン表示
    CountDownDisplay* countDownDisplay { CountDownDisplay::create() };
    countDownDisplay->setLocalZOrder(Priority::COUNT_DOWN_DISPLAY);
    this->addChild(countDownDisplay);
    _countDownDisplay = countDownDisplay;
    if (DungeonSceneManager::getInstance()->existsStopWatch()) _countDownDisplay->slideIn();
    
    // イベント処理クラスにコールバック設定
    eventTask->_onEventStart = CC_CALLBACK_0(DungeonScene::onEventStart, this);
    eventTask->_onEventFinished = CC_CALLBACK_0(DungeonScene::onEventFinished, this);
    
    // 敵処理クラスにコールバック設定
    enemyTask->onAllEnemyRemoved = CC_CALLBACK_0(DungeonScene::onAllEnemyRemoved, this);
    
    // オブジェクトリストにコールバック設定
    mapLayer->getMapObjectList()->_onLostMainCharacterHP = CC_CALLBACK_0(DungeonSceneEventHandler::onLostMainCharacterHP, _handler);
    
    // リスナにコールバック設定
    _listener->onCursorKeyPressed = [playerControlTask, party](const Key& key){playerControlTask->turn(key, party);};
    _listener->onEnterKeyPressed = [playerControlTask, party]{playerControlTask->onEnterKeyPressed(party);};
    _listener->onMenuKeyPressed = CC_CALLBACK_0(DungeonScene::onMenuKeyPressed, this);
    
    // Trigger::INITを実行
    eventTask->runEvent(mapLayer->getMapObjectList()->getEventIds(Trigger::INIT), [this, loadingLayer](){this->onInitEventFinished(loadingLayer);});
}

// Trigger::INITのイベント実行後
void DungeonScene::onInitEventFinished(LoadingLayer* loadingLayer)
{
    _cameraTask->setTarget( _party->getMainCharacter() );
    
    _enemyTask->start(this->getData()->getInitialLocation().map_id);
    
    // ローディング終了
    loadingLayer->onLoadFinished();
    
    // 装備イベントの実行
    DungeonSceneManager::getInstance()->getEquipItemEvent()->updateEquipmentEvent();
    
    // Trigger::AFTER_INITを実行
    _eventTask->runEvent(_mapLayer->getMapObjectList()->getEventIds(Trigger::AFTER_INIT), CC_CALLBACK_0(DungeonScene::onAfterInitEventFinished, this));
}

// Trigger::AFTER_INITのイベント終了時
void DungeonScene::onAfterInitEventFinished()
{
    _eventTask->runEvent({this->getData()->getInitialEventId()}, CC_CALLBACK_0(DungeonScene::onPassedEventFinished, this));
}

// マップ移動前に渡されたイベント終了時
void DungeonScene::onPassedEventFinished()
{
    // マップ名通知
    NotificationManager::getInstance()->notifyMapName(this->getData()->getInitialLocation().map_id);
    
    _playerControlTask->setControlEnable(true, _party);
}

// メニューキー押したとき
void DungeonScene::onMenuKeyPressed()
{
    // イベント中なら無視
    if (_eventTask->isEventRunning() || _eventTask->existsEvent()) return;
    
    // 敵が存在すれば無視
    if (_enemyTask->existsEnemy()) {
        SoundManager::getInstance()->playSE(Resource::SE::FAILURE, 0.5);
        return;
    }
    
    // リスナーを停止
    _listener->setEnabled(false);
    
    // カウントダウンしてれば停止
    DungeonSceneManager::getInstance()->pauseStopWatch();
    
    // パーティの位置をセット
    PlayerDataManager::getInstance()->getLocalData()->setLocation(DungeonSceneManager::getInstance()->getParty()->getMembersData());
    
    // スクショをとる
    RenderTexture* renderTexture { RenderTexture::create(WINDOW_WIDTH, WINDOW_HEIGHT) };
    renderTexture->beginWithClear(0.f, 0.f, 0.f, 0.f);
    this->visit();
    renderTexture->end();
    
    DungeonMenuScene* menu { DungeonMenuScene::create(renderTexture->getSprite()) };
    menu->onBackToTitleSelected = CC_CALLBACK_0(DungeonScene::onBackToTitleSelected, this);
    menu->onPopMenuScene = CC_CALLBACK_0(DungeonScene::onPopMenuScene, this);
    
    // メニューシーンをプッシュ
    Director::getInstance()->pushScene(menu);
}

// メニューシーンから戻ってきた時
void DungeonScene::onPopMenuScene()
{
    // カウントダウンをしれてば再開
    DungeonSceneManager::getInstance()->startStopWatch();
    
    // 装備チェンジイベント実行可能状態に
    DungeonSceneManager::getInstance()->onReturnFromDungeonMenuScene = true;
    
    // 操作可能に戻す
    _listener->setEnabled(true);
}

// メニューシーンでタイトルへ戻るを選択した時
void DungeonScene::onBackToTitleSelected()
{
    Director::getInstance()->popScene();
    this->onExitDungeon();
    Director::getInstance()->replaceScene(TitleScene::create());
}

// 敵が全ていなくなった時
void DungeonScene::onAllEnemyRemoved()
{
    // スタミナバーを隠す
    _staminaBar->slideOut();
    
    // 主人公疲労のBGMを消す
    if (SoundManager::getInstance()->isPlaying(Resource::BGM::TIRED)) SoundManager::getInstance()->stopBGM(Resource::BGM::TIRED);
}

// ダンジョンシーンから他のシーンへ移動する時
void DungeonScene::onExitDungeon()
{
    DungeonSceneManager::destroy();
    SoundManager::getInstance()->stopBGMAll();
}

// イベントを実行する時
void DungeonScene::onEventStart()
{
    // プレイヤーの操作を無効に
    _playerControlTask->setControlEnable(false, _party);
    
    // 全てのオブジェクトにイベント開始を通知
    _mapLayer->getMapObjectList()->onEventStart();
    
    // スタミナの増減を一時停止
    DungeonSceneManager::getInstance()->getStamina()->setPaused(true);
    
    // キーコンフィグを無効
    _configListener->setKeyconfigEnabled(false);
    
    // カウントダウンしてれば停止
    DungeonSceneManager::getInstance()->pauseStopWatch();
}

// イベントキューが空になった時
void DungeonScene::onEventFinished()
{
    // プレイヤーの操作を有効に
    _playerControlTask->setControlEnable(true, _party);
    
    // 全てのオブジェクトにイベント終了を通知
    _mapLayer->getMapObjectList()->onEventFinished();
    
    // スタミナの増減を再開
    DungeonSceneManager::getInstance()->getStamina()->setPaused(false);
    
    // キーコンフィグを有効
    _configListener->setKeyconfigEnabled(true);
    
    // カウントダウンをしれてば再開
    DungeonSceneManager::getInstance()->startStopWatch();
}

// バトル開始時
void DungeonScene::onBattleStart(Battle* battle)
{
    if (_handler) _handler->onBattleStart(battle);
}

// バトル終了時
void DungeonScene::onBattleFinished(Battle* battle)
{
    if (_handler) _handler->onBattleFinished(battle);
}

// pushされたシーンが表示される時
void DungeonScene::onEnterPushedScene()
{
    // リスナーを停止
    if (_listener) _listener->setEnabled(false);
    
    // カウントダウンしてれば停止
    DungeonSceneManager::getInstance()->pauseStopWatch();
}

// pushされたシーンから戻ってくる特
void DungeonScene::onExitPushedScene()
{
    // カウントダウンをしれてば再開
    DungeonSceneManager::getInstance()->startStopWatch();
    
    // 操作可能に戻す
    if (_listener) _listener->setEnabled(true);
}

// データクラスを取得
DungeonSceneData* DungeonScene::getData() const { return dynamic_cast<DungeonSceneData*>(_data); }

// カウントダウンディスプレイを取得
CountDownDisplay* DungeonScene::getCountDownDisplay()
{
    return _countDownDisplay;
}
