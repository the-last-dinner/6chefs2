//
//  StartUpScene.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/13.
//
//

#include "Scenes/StartUpScene.h"
#include "Scenes/TitleScene.h"
#include "Datas/Scene/StartUpSceneData.h"
#include "Layers/EventListener/ConfigEventListenerLayer.h"
#include "Layers/LoadingLayer.h"
#include "Utils/JsonUtils.h"
#include "Utils/CsvUtils.h"

// 初期化
bool StartUpScene::init()
{
    if (!BaseScene::init(StartUpSceneData::create())) return false;
    
    this->configListener->setKeyconfigEnabled(false);
    
    // マスターデータ準備
    CsvDataManager::getInstance();
    
    // 暗号化が必要な場合は暗号化
    if (DebugManager::getInstance()->getCryptTrigger() && DebugManager::getInstance()->isPlainData())
    {
        this->ecnryptCsvFiles();
        this->encryptSaveFiles();
        this->encryptEventScripts();
        DebugManager::getInstance()->setOffCryptTrigger();
        DebugManager::getInstance()->setOffPlainData();
    }
    
    // セーブデータ準備
    PlayerDataManager::getInstance();
    
    // キーコンフィグの取得
    KeyconfigManager::getInstance()->setCursorKey(PlayerDataManager::getInstance()->getGlobalData()->getCursorKey());
    KeyconfigManager::getInstance()->setDashKey(PlayerDataManager::getInstance()->getGlobalData()->getDashKey());
    
    return true;
}

// シーン生成直後
void StartUpScene::onEnter()
{
    BaseScene::onEnter();
}

// データ読み込み後
void StartUpScene::onPreloadFinished(LoadingLayer *loadingLayer)
{
    // ローディング終了
    loadingLayer->onLoadFinished();
    
    // ロゴ生成
    Sprite* logo {Sprite::createWithSpriteFrameName("the_last_dinner_log.png")};
    logo->setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    logo->setScale(0.8f);
    logo->setOpacity(0);
    logo->setZOrder(1000);
    this->addChild(logo);
    
    // 効果音
    SoundManager::getInstance()->playSE(Resource::SE::LOGO, 0.3f);
    
    // ロゴのアニメーション
    logo->runAction(Sequence::createWithTwoActions(FadeIn::create(0.5f),EaseCubicActionOut::create(TintTo::create(1.0f, Color3B::RED))));
    
    // シーンのアニメーション
    this->runAction(Sequence::create(DelayTime::create(2.0f), TargetedAction::create(logo,FadeOut::create(1.0f)),CallFunc::create([](){Director::getInstance()->replaceScene(TitleScene::create());}), nullptr));
}

// セーブデータの暗号化
void StartUpScene::encryptSaveFiles()
{
    vector<string> files= {
        "global_template",
        "global",
        "local_template",
        "local1",
        "local2",
        "local3",
        "local4",
        "local5",
        "local6",
        "local7",
        "local8",
        "local9",
        "local10",
    };
    string path = "";
    for (string file : files)
    {
        path = FileUtils::getInstance()->fullPathForFilename("save/" + file + SAVE_EXTENSION);
        if (path != "") LastSupper::JsonUtils::enctyptJsonFile(path);
    }
}

// イベントスクリプトの暗号化
void StartUpScene::encryptEventScripts()
{
    vector<string> fileNames = CsvDataManager::getInstance()->getMapData()->getFileNameAll();
    rapidjson::Document common = LastSupper::JsonUtils::readJsonFile(FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::COMMON_EVENT));
    for (int i = 0; i < common.Size(); i++)
    {
        fileNames.push_back(common[i]["name"].GetString());
    }
    string path = "";
    for(string file : fileNames)
    {
        path = FileUtils::getInstance()->fullPathForFilename("event/" + file + ES_EXTENSION);
        LastSupper::JsonUtils::enctyptJsonFile(path);
    }
}

// CSVの暗号化
void StartUpScene::ecnryptCsvFiles()
{
    vector<string> files = {
        "character",
        "chapter",
        "item",
        "map",
        "trophy",
    };
    string path = "";
    for(string file : files)
    {
        path = FileUtils::getInstance()->fullPathForFilename("csv/" + file + CSV_EXTENSION);
        CsvUtils::encryptCsvToJson(path);
    }
}