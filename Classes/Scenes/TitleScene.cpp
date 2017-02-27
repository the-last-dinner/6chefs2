//
//  TitleScene.cpp
//  LastSupper
//
//  Created by Kohei on 2015/06/05.
//
//

#include "Scenes/TitleScene.h"
#include "Scenes/DungeonScene.h"

#include "Datas/Scene/TitleSceneData.h"
#include "Datas/Scene/DungeonSceneData.h"

#include "Layers/LoadingLayer.h"
#include "Layers/Menu/SaveDataSelector.h"
#include "Layers/Menu/TitleMainMenuLayer.h"
#include "Layers/Menu/TrophyListLayer.h"

#include "Models/PlayerData/GlobalPlayerData.h"

// コンストラクタ
TitleScene::TitleScene(){FUNCLOG}

// デストラクタ
TitleScene::~TitleScene(){FUNCLOG}

// 初期化
bool TitleScene::init()
{
    if(!BaseScene::init(TitleSceneData::create())) return false;
    
    return true;
}

// シーン切り替え完了時
void TitleScene::onEnter()
{
    BaseScene::onEnter();
}

// リソースロード終了後の処理
void TitleScene::onPreloadFinished(LoadingLayer* loadingLayer)
{
    // ローディング終了
    loadingLayer->onLoadFinished();
    
    // メインメニューレイヤーを生成
    TitleMainMenuLayer* mainMenu {TitleMainMenuLayer::create()};
    this->addChild(mainMenu);
    
    // メインメニューのイベントをリッスン
    mainMenu->onStartSelected = CC_CALLBACK_0(TitleScene::onStartSelected, this);
    mainMenu->onContinueSelected = CC_CALLBACK_0(TitleScene::onContinueSelected, this);
    mainMenu->onExitSelected = CC_CALLBACK_0(TitleScene::onExitSelected, this);
    mainMenu->onTrophySelected = CC_CALLBACK_0(TitleScene::onTrophyListSelected, this);
    mainMenu->onSpecialRoomSelected = CC_CALLBACK_0(TitleScene::onSpecialRoomSelected, this);
    
    mainMenu->show();
    this->mainMenu = mainMenu;
    
    // セーブデータ選択レイヤーを生成
    SaveDataSelector* saveDataSelector { SaveDataSelector::create(false) };
    this->addChild(saveDataSelector);
    
    // セーブデータ選択レイヤーのイベントをリッスン
    saveDataSelector->onSaveDataSelectCancelled = CC_CALLBACK_0(TitleScene::onSaveDataSelectCancelled, this);
    saveDataSelector->setVisible(false);
    this->saveDataSelector = saveDataSelector;
    
    // BGM
    SoundManager::getInstance()->playBGM("title_bgm.mp3", true, 0.7f);
}

// 最初からが選ばれた時
void TitleScene::onStartSelected()
{
    SoundManager::getInstance()->stopBGMAll();
    SoundManager::getInstance()->playSE(Resource::SE::GAME_START);
	PlayerDataManager::getInstance()->setGameStart(0);
    Director::getInstance()->replaceScene(DungeonScene::create(DungeonSceneData::create(PlayerDataManager::getInstance()->getLocalData()->getLocation())));
}

// 続きからが選ばれた時
void TitleScene::onContinueSelected()
{
    SoundManager::getInstance()->playSE(Resource::SE::TITLE_ENTER);
	this->mainMenu->hide();
	this->saveDataSelector->show();
}

// 終了が選ばれた時
void TitleScene::onExitSelected()
{
    SoundManager::getInstance()->playSE(Resource::SE::BACK);
	Director::getInstance()->end();
}

// セーブデータ選択をキャンセルした時
void TitleScene::onSaveDataSelectCancelled()
{
    SoundManager::getInstance()->playSE(Resource::SE::BACK);
	this->saveDataSelector->hide();
	this->mainMenu->show();
}

// おまけ部屋が選択された時
void TitleScene::onSpecialRoomSelected()
{
    SoundManager::getInstance()->playSE(Resource::SE::LOAD);
    PlayerDataManager::getInstance()->setGameStart(SPECIAL_ROOM_SAVE_ID);
    
    // 保存されているBGMの再生
    SoundManager::getInstance()->stopBGMAll();
    vector<string> bgms {PlayerDataManager::getInstance()->getLocalData()->getBgmAll()};
    for(string bgm : bgms)
    {
        SoundManager::getInstance()->playBGM(bgm);
    }
    
    // シーン移動
    Director::getInstance()->replaceScene(DungeonScene::create(DungeonSceneData::create(PlayerDataManager::getInstance()->getLocalData()->getLocation())));
}

#pragma mark -
#pragma mark TrophyListLayer

// トロフィーリストを生成
void TitleScene::createTrophyListLayer()
{
    TrophyListLayer* trophyList { TrophyListLayer::create() };
    trophyList->onTrophyListCanceled = CC_CALLBACK_0(TitleScene::onTrophyListCanceled, this);
    trophyList->setVisible(false);
    this->addChild(trophyList);
    this->trophyList = trophyList;
}

// トロフィーリストを選択した時
void TitleScene::onTrophyListSelected()
{
    // トロフイーリストレイヤーを作成
    SoundManager::getInstance()->playSE(Resource::SE::TITLE_ENTER);
    if(!this->trophyList)
    {
        this->createTrophyListLayer();
    }
    this->mainMenu->hide();
    this->trophyList->show();
}

// トロフィーリストをキャンセル時
void TitleScene::onTrophyListCanceled()
{
    SoundManager::getInstance()->playSE(Resource::SE::BACK);
    this->trophyList->hide();
    this->mainMenu->show();
}

#pragma mark -
#pragma mark Interface
LoadingLayer* TitleScene::createLoadingLayer()
{
    return LoadingLayer::create(Color4B(0, 0, 0, 0));
}
