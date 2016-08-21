//
//  DungeonMenuScene.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/10/04.
//
//

#include "Scenes/DungeonMenuScene.h"
#include "Layers/Menu/DungeonMainMenuLayer.h"
#include "Layers/Menu/SaveDataSelector.h"
#include "Layers/Menu/ItemMenuLayer.h"
#include "Layers/Menu/DocumentMenuLayer.h"
#include "Managers/DungeonSceneManager.h"
#include "Models/StopWatch.h"

// クラス変数
const string DungeonMenuScene::MAIN_LAYER_NAME = "mainMenuLayer";
const string DungeonMenuScene::SAVE_LAYER_NAME = "saveMenuLayer";
const string DungeonMenuScene::ITEM_LAYER_NAME = "itemMenuLayer";
const string DungeonMenuScene::DOCUMENT_LAYER_NAME = "documentMenuLayer";

// コンストラクタ
DungeonMenuScene::DungeonMenuScene(){FUNCLOG}

// デストラクタ
DungeonMenuScene::~DungeonMenuScene(){FUNCLOG}

// 初期化
bool DungeonMenuScene::init(Sprite* screen)
{
    FUNCLOG;
    // スクショを生成
    screen->setScale(Director::getInstance()->getWinSize().width / screen->getContentSize().width);
    screen->setPosition(WINDOW_CENTER);
    this->addChild(screen);
    
    // メインメニューレイヤーを生成
    this->createMainMenu();
    
    return true;
}

// メインメニューレイヤー生成
void DungeonMenuScene::createMainMenu()
{
    FUNCLOG
    // すでに存在すれば削除
    if(this->getChildByName(MAIN_LAYER_NAME))
    {
        this->menuIndex = this->mainMenu->getMenuIndex();
        this->removeChildByName(MAIN_LAYER_NAME);
    }
    // 生成
    DungeonMainMenuLayer* menu = DungeonMainMenuLayer::create();
    menu->setName(MAIN_LAYER_NAME);
    menu->onSaveMenuSelected = CC_CALLBACK_0(DungeonMenuScene::onSaveMenuSelected, this);
    menu->onMenuHidden = CC_CALLBACK_0(DungeonMenuScene::onMenuHidden, this);
    menu->onItemMenuSelected = CC_CALLBACK_0(DungeonMenuScene::onItemMenuSelected, this);
    menu->onCharacterMenuSelected = CC_CALLBACK_0(DungeonMenuScene::onDocumentMenuSelected, this);
    menu->onBackToTitleConfirmed = CC_CALLBACK_0(DungeonMenuScene::onBackToTitleConfirmed, this);
    menu->setDefaultMenuIndex(this->menuIndex);
    this->addChild(menu);
    this->mainMenu = menu;
    this->mainMenu->show();
}

// セーブデータ選択レイヤーの生成
void DungeonMenuScene::createSaveMenu()
{
    FUNCLOG
    // すでに存在すれば削除
    if(this->getChildByName(SAVE_LAYER_NAME))
    {
        this->removeChildByName(SAVE_LAYER_NAME);
    }
    // 生成
    SaveDataSelector* saveDataSelector { SaveDataSelector::create(true) };
    saveDataSelector->setName(SAVE_LAYER_NAME);
    this->addChild(saveDataSelector);
    // セーブデータ選択レイヤーのイベントをリッスン
    saveDataSelector->onSaveDataSelectCancelled = CC_CALLBACK_0(DungeonMenuScene::onSaveDataSelectCancelled, this);
    this->saveDataSelector = saveDataSelector;
}

// アイテムメニューレイヤーの生成
void DungeonMenuScene::createItemMenu()
{
    FUNCLOG
    // すでに存在すれば削除
    if(this->getChildByName(ITEM_LAYER_NAME))
    {
        this->removeChildByName(ITEM_LAYER_NAME);
    }
    // 生成
    ItemMenuLayer* itemMenu { ItemMenuLayer::create() };
    itemMenu->setName(ITEM_LAYER_NAME);
    this->addChild(itemMenu);
    itemMenu->onItemMenuCanceled = CC_CALLBACK_0(DungeonMenuScene::onItemMenuCanceled, this);
    this->itemMenu = itemMenu;
}

// キャラクターメニューレイヤーの生成
void DungeonMenuScene::createDocumentMenu()
{
    FUNCLOG
    // すでに存在すれば削除
    if(this->getChildByName(DOCUMENT_LAYER_NAME))
    {
        this->removeChildByName(DOCUMENT_LAYER_NAME);
    }
    // 生成
    DocumentMenuLayer* documentMenu { DocumentMenuLayer::create() };
    documentMenu->setName(DOCUMENT_LAYER_NAME);
    documentMenu->onCharacterMenuCanceled = CC_CALLBACK_0(DungeonMenuScene::onDocumentMenuCanceled, this);
    this->addChild(documentMenu);
    this->documentMenu = documentMenu;
}

void DungeonMenuScene::onPreloadFinished(LoadingLayer* loadingLayer){}

// 方向キーを押した時
void DungeonMenuScene::onCursorKeyPressed(const Key& key){}

// 決定キーを押した時
void DungeonMenuScene::onEnterKeyPressed(){}


// キーを押し続けている時
void DungeonMenuScene::intervalInputCheck(const vector<Key>& keys){}

// メニューキー押したとき
void DungeonMenuScene::onMenuKeyPressed(){}

//メニューが削除されたとき
void DungeonMenuScene::onMenuHidden()
{
    FUNCLOG
    if(this->onPopMenuScene)
    {
        this->onPopMenuScene();
    }
    Director::getInstance()->popScene();
}

#pragma mark -
#pragma mark SaveDataSelecter

// セーブメニューが選択された時
void DungeonMenuScene::onSaveMenuSelected()
{
    FUNCLOG
    // カウントダウン中とおまけ部屋は開けなくする
    if (DungeonSceneManager::getInstance()->existsStopWatch() ||
        PlayerDataManager::getInstance()->getLocalData()->isSpecialRoom())
    {
        SoundManager::getInstance()->playSE("failure.mp3");
        return;
    }
    
    SoundManager::getInstance()->playSE("cursorMove.mp3");
    this->mainMenu->hide();
    this->createSaveMenu();
    this->saveDataSelector->show();
}

// セーブデータ選択をキャンセルした時
void DungeonMenuScene::onSaveDataSelectCancelled()
{
    FUNCLOG
    SoundManager::getInstance()->playSE("back.mp3");
    runAction(Sequence::createWithTwoActions(
        CallFunc::create([this](){this->saveDataSelector->hide();}),
        CallFunc::create([this](){this->createMainMenu();})
    ));
}

#pragma mark -
#pragma mark ItemMenu

// アイテムメニューが選択された時
void DungeonMenuScene::onItemMenuSelected()
{
    FUNCLOG
    SoundManager::getInstance()->playSE("cursorMove.mp3");
    this->mainMenu->hide();
    this->createItemMenu();
    this->itemMenu->show();
}

// アイテムメニューでキャンセルを押した時
void DungeonMenuScene::onItemMenuCanceled()
{
    FUNCLOG
    SoundManager::getInstance()->playSE("back.mp3");
    runAction(Sequence::createWithTwoActions(
        CallFunc::create([this](){this->itemMenu->hide();}),
        CallFunc::create([this](){this->createMainMenu();})
    ));
}

#pragma mark -
#pragma mark DocumentMenu

// 資料メニューが選択された時
void DungeonMenuScene::onDocumentMenuSelected()
{
    FUNCLOG
    // おまけ部屋は開けない
    if (PlayerDataManager::getInstance()->getLocalData()->isSpecialRoom())
    {
        SoundManager::getInstance()->playSE("failure.mp3");
        return;
    }
    
    SoundManager::getInstance()->playSE("cursorMove.mp3");
    this->mainMenu->hide();
    this->createDocumentMenu();
    this->documentMenu->show();
}

// 資料メニューでキャンセルされた時
void DungeonMenuScene::onDocumentMenuCanceled()
{
    FUNCLOG
    SoundManager::getInstance()->playSE("back.mp3");
    runAction(Sequence::createWithTwoActions(
        CallFunc::create([this](){this->documentMenu->hide();}),
        CallFunc::create([this](){this->createMainMenu();})
    ));
}

#pragma mark -
#pragma mark MainMenu

// タイトルへ戻るを承認した時
void DungeonMenuScene::onBackToTitleConfirmed()
{
    this->mainMenu->hide();
    SoundManager::getInstance()->playSE("back.mp3");
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(DungeonMainMenuLayer::SLIDE_TIME), CallFunc::create([this]
    {
        if(this->onBackToTitleSelected) this->onBackToTitleSelected();
    })));
}