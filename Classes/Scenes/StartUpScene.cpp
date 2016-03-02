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
#include "Layers/LoadingLayer.h"
#include "Utils/JsonUtils.h"

// 初期化
bool StartUpScene::init()
{
    if (!BaseScene::init(StartUpSceneData::create())) return false;
    
    // CSVデータの作成
    if (!DebugManager::getInstance()->isCryptedCsvData())
    {
        this->ecnryptCsvFiles();
    }
    CsvDataManager::getInstance();
    
    // セーブデータの生成
    if (!DebugManager::getInstance()->isCryptedSaveData())
    {
        this->encryptSaveFiles();
    }
    PlayerDataManager::getInstance();
    
    // イベントスクリプトの暗号化
    if (!DebugManager::getInstance()->isCryptedEventScript())
    {
        this->encryptEventScripts();
    }
    
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
    DebugManager::getInstance()->setCryptedSaveData();
}

// イベントスクリプトの暗号化
void StartUpScene::encryptEventScripts()
{
    vector<string> fileNames = CsvDataManager::getInstance()->getMapFileNameAll();
    string path = "";
    for(string file : fileNames)
    {
        path = FileUtils::getInstance()->fullPathForFilename("event/" + file + ES_EXTENSION);
        LastSupper::JsonUtils::enctyptJsonFile(path);
    }
    DebugManager::getInstance()->setCryptedEventScript();
}

// CSVの暗号化
void StartUpScene::ecnryptCsvFiles()
{
    vector<string> files = {
        "chapter",
        "item",
        "map",
        "trophy",
        "character1",
        "character2",
        "character3"
    };
    string path = "";
    for(string file : files)
    {
        path = FileUtils::getInstance()->fullPathForFilename("csv/" + file + CSV_EXTENSION);
        // ファイル読み込み
        ifstream ifs(path);
        if (ifs.fail())
        {
            CCLOG("%s is missing.", path.c_str());
            return;
        }
        
        // 文字列を暗号化
        string str;
        getline(ifs, str);
        ofstream ofs;
        ofs.open(path);
        while(getline(ifs, str))
        {
            //LastSupper::StringUtils::encryptXor(jsonStr);
            for(int i = 0; i < strlen(str.c_str()); i++)
            {
                str[i] ^= C_KEY;
            }
            ofs << str << endl;
        }
        ifs.close();
        ofs.close();
    }
    DebugManager::getInstance()->setCryptedCsvData();
}