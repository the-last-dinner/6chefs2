//
//  DungeonCameraScene.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/04.
//
//

#include "Scenes/DungeonCameraScene.h"

#include "Datas/Scene/DungeonCameraSceneData.h"

#include "Effects/AmbientLightLayer.h"

#include "Layers/Dungeon/TiledMapLayer.h"
#include "Layers/LoadingLayer.h"

#include "MapObjects/MapObjectList.h"

#include "Tasks/CameraTask.h"
#include "Tasks/EventTask.h"

#include "Managers/DungeonSceneManager.h"

// create関数
DungeonCameraScene* DungeonCameraScene::create(DungeonCameraSceneData* data, GameEvent* event, EventFinishCallback callback)
{
    DungeonCameraScene* p { new(nothrow) DungeonCameraScene() };
    if(p && p->init(data, event, callback))
    {
        p->autorelease();
        return p;
    }
    else
    {
        delete p;
        p = nullptr;
        return nullptr;
    }
}

// コンストラクタ
DungeonCameraScene::DungeonCameraScene() { FUNCLOG };

// デストラクタ
DungeonCameraScene::~DungeonCameraScene() { FUNCLOG };

// 初期化
bool DungeonCameraScene::init(DungeonCameraSceneData* data, GameEvent* event, EventFinishCallback callback)
{
    if(!BaseScene::init(data)) return false;
    
    this->callback = callback;
    this->event = event;
    
    return true;
}

// シーン切り替え終了時
void DungeonCameraScene::onEnter()
{
    DungeonScene::onEnter();
}

// プリロード終了時
void DungeonCameraScene::onPreloadFinished(LoadingLayer* loadingLayer)
{
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
    
    // カメラ処理クラス生成
    CameraTask* cameraTask {CameraTask::create()};
    this->addChild(cameraTask);
    this->cameraTask = cameraTask;
    
    // イベント処理クラス生成
    EventTask* eventTask { EventTask::create() };
    this->addChild(eventTask);
    this->eventTask = eventTask;
    
    // カメラの設定
    // IDのオブジェクトが存在すれば、そのオブジェクトが常に中心に来るようにする
    if(this->getData()->getTargetId() != etoi(ObjectID::UNDIFINED))
    {
        cameraTask->setTarget(mapLayer->getMapObjectList()->getMapObject(this->getData()->getTargetId()));
    }
    else
    {
        cameraTask->setCenter(Point(this->getData()->getInitialLocation().x, this->getData()->getInitialLocation().y));
    }
    
    // Trigger::INITを実行
    eventTask->runEvent(mapLayer->getMapObjectList()->getEventIds(Trigger::INIT), [this, loadingLayer](){this->onInitEventFinished(loadingLayer);});
}

// Trigger::INITのイベントが終了した時
void DungeonCameraScene::onInitEventFinished(LoadingLayer* loadingLayer)
{
    // ローディング終了
    loadingLayer->onLoadFinished();
    
    // オブジェクトの自動移動開始
    this->mapLayer->getMapObjectList()->moveStartAllObjects();
    
    // Trigger::AFTER_INITを実行
    this->eventTask->runEvent(mapLayer->getMapObjectList()->getEventIds(Trigger::AFTER_INIT), CC_CALLBACK_0(DungeonCameraScene::onAfterInitEventFinished, this));
}

// Trigger::AFTER_INITが終了した時
void DungeonCameraScene::onAfterInitEventFinished()
{
    // 指定したイベントを実行
    this->eventTask->runEvent(this->event, CC_CALLBACK_0(DungeonCameraScene::onCameraEventFinished, this));
}

// 渡されたイベントを終了した時
void DungeonCameraScene::onCameraEventFinished()
{
    if(this->callback) this->callback();
}

// データ取得
DungeonCameraSceneData* DungeonCameraScene::getData() const
{
    return dynamic_cast<DungeonCameraSceneData*>(this->data);
}