//
//  BaseScene.cpp
//  LastSupper
//
//  Created by Kohei on 2015/06/05.
//
//

#include "Scenes/BaseScene.h"

#include "Datas/Scene/SceneData.h"

#include "Layers/LoadingLayer.h"
#include "Layers/EventListener/ConfigEventListenerLayer.h"

#include "Managers/NotificationManager.h"

// コンストラクタ
BaseScene::BaseScene() {}

// デストラクタ
BaseScene::~BaseScene()
{
	CC_SAFE_RELEASE_NULL(_data);
}

// シーン共通初期化
bool BaseScene::init(SceneData* data)
{
	if(!Scene::init()) return false;
    
    // コンフィグ用のイベントリスナーを登録
    ConfigEventListenerLayer* configListener { ConfigEventListenerLayer::create() };
    this->addChild(configListener);
    _configListener = configListener;
	
	// データクラスをセットしretain
	_data = data;
	CC_SAFE_RETAIN(_data);
    
	return true;
}

// シーンの切り替え完了時
void BaseScene::onEnter()
{
    Scene::onEnter();
    
    // 通知がキューに残っていたら、通知を開始する
    NotificationManager::getInstance()->notifyRemainsInQueue();
    
    // すでにプリロード済みなら無視
    if(_isPreloaded) return;
    
    // ロード画面レイヤー
    LoadingLayer* loadingLayer { this->createLoadingLayer() };
    loadingLayer->setLocalZOrder(Priority::LOADING_LAYER);
    this->addChild(loadingLayer);
    
    // プリロード開始
    _data->preloadResources([this, loadingLayer](float percentage)
    {
        if(percentage == 1.f)
        {
            _isPreloaded = true;
            
            // プリロード完了時にコールバック
            this->onPreloadFinished(loadingLayer);
        }
    });
}

// ローディングレイヤを生成
LoadingLayer* BaseScene::createLoadingLayer()
{
    return LoadingLayer::create();
}
