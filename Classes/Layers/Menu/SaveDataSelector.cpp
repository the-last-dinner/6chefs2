//
//  SaveDataSelector.cpp
//  LastSupper
//
//  Created by Kohei on 2015/07/20.
//
//

#include "Layers/Menu/SaveDataSelector.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"

#include "Scenes/DungeonScene.h"
#include "Datas/Scene/DungeonSceneData.h"
#include "Datas/MapObject/CharacterData.h"
#include "UI/NotificationBand.h"
#include "Managers/CsvDataManager.h"
#include "Managers/ConfigDataManager.h"

// 定数
const int SaveDataSelector::MAX_FRIENDSHIP_COUNT = 5;
const int SaveDataSelector::MAX_PARTY_DISPLAY_COUNT = 4;

// コンストラクタ
SaveDataSelector::SaveDataSelector(){FUNCLOG}

// デストラクタ
SaveDataSelector::~SaveDataSelector(){FUNCLOG}

// 初期化
bool SaveDataSelector::init(bool write = false)
{
	FUNCLOG
	if(!MenuLayer::init(2, MAX_SAVE_COUNT / 2)) return false;
	
    // フラグをセット
    this->writeFlag = write;
    this->comfirmFlag = false;
    
	// 黒い背景を生成
	Sprite* black = Sprite::create();
	black->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	black->setColor(Color3B::BLACK);
	black->setPosition(WINDOW_CENTER);
	this->addChild(black);
	
	// セーブデータインデックスを取得
	vector<SaveIndex> saveIndexies = this->createSaveIndexies();
    
    SaveIndex data {};
	for(int i = 0; i < saveIndexies.size();i++)
	{
        Node* panel {this->createSavePanel(saveIndexies.at(i))};
        panel->setTag(i);
        this->addChild(panel);
        this->menuObjects.push_back(panel);
	}
    
    // デフォルトセレクト
	this->setCascadeOpacityEnabled(true);
    LocalPlayerData* local = PlayerDataManager::getInstance()->getLocalData();
    int id = local != nullptr ? local->getLocalId() : 1;
    int index = (id <= 0) ? 0 : id - 1;
    this->setSelectedIndex(index);
	this->onIndexChanged(index, false);
	
	return true;
}

// セーブメニューのパネル一つ分を作成
Node* SaveDataSelector::createSavePanel(SaveIndex saveIndex)
{
    int i = saveIndex.dataId - 1;
    float margin {15.f};
    
    Sprite* panel {Sprite::createWithSpriteFrameName("save_frame.png")};
    Size panelSize {panel->getContentSize()};
    panel->setPosition((panelSize.width / 2) * (2 * (i % 2) + 1), WINDOW_HEIGHT - (panelSize.height / 2) * (2 * (i / 2) + 1));
    
    // 番号
    Label* num = Label::createWithTTF(to_string(saveIndex.dataId), Resource::Font::MESSAGE, panelSize.height / 5);
    num->setPosition(Point(num->getContentSize().width / 2 + margin, panel->getContentSize().height - num->getContentSize().height / 2 - margin));
    panel->addChild(num);
    
    // チャプター
    Label* name = Label::createWithTTF(saveIndex.chapter, Resource::Font::MESSAGE, panelSize.height / 5);
    name->setPosition(Point(name->getContentSize().width / 2 + num->getContentSize().width + margin * 2, name->getContentSize().height * 2 + margin));
    panel->addChild(name);
    
    // マップ名
    Label* mapName = Label::createWithTTF(saveIndex.mapName, Resource::Font::MESSAGE, panelSize.height / 6);
    mapName->setPosition(Point(panelSize.width - mapName->getContentSize().width / 2 - margin, panelSize.height - mapName->getContentSize().height/2 - margin));
    panel->addChild(mapName);
    
    // プレイ時間
    Label* time = Label::createWithTTF(saveIndex.playTime, Resource::Font::MESSAGE, panelSize.height / 6);
    time->setPosition(Point(panelSize.width - time->getContentSize().width / 2 - margin, time->getContentSize().height / 2 + margin));
    panel->addChild(time);
    
    // セーブ回数
    Label* count = Label::createWithTTF(saveIndex.saveCount, Resource::Font::MESSAGE, panelSize.height / 6);
    count->setPosition(Point(panelSize.width - count->getContentSize().width / 2 - margin, time->getContentSize().height + time->getContentSize().height / 2 + margin * 2));
    panel->addChild(count);
    
    // 下部分
    if (ConfigDataManager::getInstance()->getMasterConfigData()->isDisplay(MasterConfigData::FRIENDSHIP))
    {
        // 友好度を表示 FIXME: 5個までなの微妙な気がする
        for (int i = 0; i < saveIndex.friendship; i++)
        {
            Sprite* heart {Sprite::createWithSpriteFrameName("heart_pink.png")};
            float scale = 0.30;
            heart->setScale(scale);
            heart->setOpacity(128);
            Size heartSize {Size(heart->getContentSize().width * scale - 5, heart->getContentSize().height * scale - 25)};
            heart->setPosition(i * (heartSize.width + 8) + heartSize.width + num->getContentSize().width + 10, heartSize.height / 2 + margin);
            panel->addChild(heart);
            if (i >= SaveDataSelector::MAX_FRIENDSHIP_COUNT) break;
        }
    }
    else
    {
        // パーティーメンバーを表示
        for (int i = 0; i < saveIndex.party.size(); i ++)
        {
            if (SpriteFrameCache::getInstance()->getSpriteFrameByName(saveIndex.party[i]))
            {
                Sprite* dotimg {Sprite::createWithSpriteFrameName(saveIndex.party[i])};
                Size dotSize {dotimg->getContentSize()};
                dotimg->setPosition((i + 1) * (dotSize.width / 2 + 30) + num->getContentSize().width + 10, dotSize.height / 2 + margin / 2);
                panel->addChild(dotimg);
            }
            if (i >= SaveDataSelector::MAX_PARTY_DISPLAY_COUNT) break;
        }
    }
    
    // 不透明度を半分にしておく
    panel->setCascadeOpacityEnabled(true);
    panel->setOpacity(100);
    return panel;
}

// セーブデータを取得
vector<SaveDataSelector::SaveIndex> SaveDataSelector::createSaveIndexies()
{
    vector<SaveIndex> save_list;
    SaveIndex save = SaveIndex();
    // セーブデータを一つずつチェック
    for(int i = 1; i <= MAX_SAVE_COUNT; i++){
        LocalPlayerData* local {LocalPlayerData::create(i)};
        if(!local)
        {
            // セーブデータが存在しない
            this->existsSaveData[i-1] = false;
            save.dataId = i;
            save.chapter = "--------- NO DATA ---------";
            save.mapName = "";
            save.playTime = "";
            save.saveCount = "";
            save.friendship = 0;
            save.party = vector<string>{};
        } else {
            // セーブデータが存在する
            this->existsSaveData[i-1] = true;
            save.dataId = i;
            save.chapter = "--- " + CsvDataManager::getInstance()->getChapterData()->getName(local->getChapterId()) + " ---";
            save.mapName = LastSupper::StringUtils::getSprintf("%15s", CsvDataManager::getInstance()->getMapData()->getName(local->getLocation().map_id));
            save.playTime = local->getPlayTimeForDisplay();
            save.saveCount = LastSupper::StringUtils::getSprintf("%3s", to_string(local->getSaveCount())) + "回";
            save.friendship = local->getMaxFriendshipCount();
            save.party = local->getPartyCharaDotImgFileNames();
        }
        save_list.push_back(save);
    }
    return save_list;
}

// 表示
void SaveDataSelector::show()
{
	this->setVisible(true);
    this->setScale(0);
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 1.f)));
	this->listenerKeyboard->setEnabled(true);
}

// 非表示
void SaveDataSelector::hide()
{
	this->listenerKeyboard->setEnabled(false);
    if (this->notification) this->notification->hide();
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 0)));
	//this->setVisible(false);
}

// 選択しているindexが変わった時
void SaveDataSelector::onIndexChanged(int newIdx, bool sound)
{
    // セーブしましたがでてる場合は無視
    if(this->comfirmFlag) return;
    // 移動音
    if (sound)
    {
        SoundManager::getInstance()->playSE("cursorMove.mp3");
    }
	for(Node* obj : this->menuObjects)
	{
		if(obj->getTag() == newIdx)
		{
			obj->runAction(FadeTo::create(0.2f, 255));
		}
		else
		{
			obj->runAction(FadeTo::create(0.2f, 100));
		}
	}
}

// 決定キーを押した時
void SaveDataSelector::onEnterKeyPressed(int idx)
{
    // インクリメント
    idx++;
    // 確認時
    if (this->comfirmFlag)
    {
        if(this->onSaveDataSelectCancelled)
        {
            this->onSaveDataSelectCancelled();
        }
        this->comfirmFlag = false;
        return;
    }
    // セーブorロード
    if (this->writeFlag)
    {
        // セーブ時
        SoundManager::getInstance()->playSE("save.mp3");
        PlayerDataManager::getInstance()->save(idx);
        
        // 完了メッセージ表示
        NotificationBand* notification {NotificationBand::create("セーブが完了しました")};
        notification->setBandColor(Color3B(64,0,0));
        notification->show();
        this->addChild(notification);
        this->notification = notification;
        this->comfirmFlag = true;
    }
    else
    {
        // ロード時
        if(this->existsSaveData[idx-1])
        {
            // ロード
            SoundManager::getInstance()->playSE(Resource::SE::LOAD);
            PlayerDataManager::getInstance()->setGameStart(idx);
            LocalPlayerData* localData { PlayerDataManager::getInstance()->getLocalData() };
            
            // 保存されているBGMの再生
            SoundManager::getInstance()->stopBGMAll();
            vector<string> bgms { localData->getBgmAll() };
            for(string bgm : bgms)
            {
                SoundManager::getInstance()->playBGM(bgm);
            }
            
            // シーン移動
            DungeonSceneData* dungeonData { DungeonSceneData::create(localData->getLocation()) };
            string initEventId = localData->getInitEventId();
            if (initEventId != "") {
                dungeonData->setInitialEventId(stoi(initEventId));
            }
            PlayerDataManager::getInstance()->getLocalData()->removeInitEventId();
            Director::getInstance()->replaceScene(DungeonScene::create(dungeonData));
        }
        else
        {
            // セーブデータが存在しない
            SoundManager::getInstance()->playSE("failure.mp3");
        }
    }
    return;
}

// メニューキーを押した時
void SaveDataSelector::onMenuKeyPressed()
{
	if(this->onSaveDataSelectCancelled)
	{
		this->onSaveDataSelectCancelled();
	}
}
