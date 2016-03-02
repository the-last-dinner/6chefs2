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
#include "UI/NotificationBand.h"

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
    this->write_flag = write;
    this->comfirm_flag = false;
	// 黒い背景を生成
	Sprite* black = Sprite::create();
	black->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	black->setColor(Color3B::BLACK);
	black->setPosition(WINDOW_CENTER);
	this->addChild(black);
	
	// セーブデータリストを取得
	this->saveDatas = this->getSaveList();
	
	Point center = WINDOW_CENTER;
    float margin {15.f};
    
	for(int i = 0; i < this->saveDatas.size();i++)
	{
		SaveIndex data {this->saveDatas.at(i)};
		Sprite* panel {Sprite::createWithSpriteFrameName("save_frame.png")};
		Size panelSize {panel->getContentSize()};
		panel->setPosition((panelSize.width / 2) * (2 * (i % 2) + 1), WINDOW_HEIGHT - (panelSize.height / 2) * (2 * (i / 2) + 1));
		panel->setTag(i);
		this->addChild(panel);
		this->menuObjects.push_back(panel);
		
        // 番号
        Label* num = Label::createWithTTF(to_string(data.data_id), "fonts/cinecaption2.28.ttf", panelSize.height / 5);
        num->setPosition(Point(num->getContentSize().width / 2 + margin, panel->getContentSize().height - num->getContentSize().height / 2 - margin));
        panel->addChild(num);
        
		// チャプター
		Label* name = Label::createWithTTF(data.chapter, "fonts/cinecaption2.28.ttf", panelSize.height / 5);
		name->setPosition(Point(name->getContentSize().width / 2 + num->getContentSize().width + margin * 2, name->getContentSize().height * 2 + margin));
		panel->addChild(name);
        
		// マップ名
		Label* mapName = Label::createWithTTF(data.map_name, "fonts/cinecaption2.28.ttf", panelSize.height / 6);
		mapName->setPosition(Point(panelSize.width - mapName->getContentSize().width / 2 - margin, panelSize.height - mapName->getContentSize().height/2 - margin));
		panel->addChild(mapName);
		
		// プレイ時間
		Label* time = Label::createWithTTF(data.play_time, "fonts/cinecaption2.28.ttf", panelSize.height / 6);
		time->setPosition(Point(panelSize.width - time->getContentSize().width / 2 - margin, time->getContentSize().height / 2 + margin));
		panel->addChild(time);
        
        // セーブ回数
        Label* count = Label::createWithTTF(data.save_count, "fonts/cinecaption2.28.ttf", panelSize.height / 6);
        count->setPosition(Point(panelSize.width - count->getContentSize().width / 2 - margin, time->getContentSize().height + time->getContentSize().height / 2 + margin * 2));
        panel->addChild(count);
        
        // 友好度
        for (int i=0; i<data.friendship; i++)
        {
            Sprite* heart {Sprite::createWithSpriteFrameName("heart_pink.png")};
            float scale = 0.30;
            heart->setScale(scale);
            heart->setOpacity(128);
            Size heartSize {Size(heart->getContentSize().width * scale - 5, heart->getContentSize().height * scale - 25)};
            heart->setPosition(i * (heartSize.width + 8) + heartSize.width + num->getContentSize().width + 10, heartSize.height / 2 + margin);
            panel->addChild(heart);
        }
        
		// 不透明度を半分にしておく
		panel->setCascadeOpacityEnabled(true);
		panel->setOpacity(100);
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

// セーブデータを取得
vector<SaveDataSelector::SaveIndex> SaveDataSelector::getSaveList()
{
    vector<SaveIndex> save_list;
    SaveIndex save;
    // セーブデータを一つずつチェック
    for(int i=1; i<=MAX_SAVE_COUNT; i++){
        string file = "save/local" + to_string(i) + SAVE_EXTENSION;
        LocalPlayerData* local {LocalPlayerData::create(i)};
        if(!local)
        {
            // セーブデータが存在しない
            this->existsSaveData[i-1] = false;
            save = SaveIndex(i, "--------- NO DATA ---------", "", "", "", 0);
            
        } else {
            // セーブデータが存在する
            this->existsSaveData[i-1] = true;
            save = SaveIndex(
                             i,
                             "--- " + CsvDataManager::getInstance()->getChapterName(local->getChapterId()) + " ---",
                             LastSupper::StringUtils::getSprintf("%15s", CsvDataManager::getInstance()->getMapName(local->getLocation().map_id)),
                             local->getPlayTimeForDisplay(),
                             LastSupper
                             ::StringUtils::getSprintf("%3s", to_string(local->getSaveCount())) + "回",
                             local->getMaxFriendshipCount()
                             );
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
    if(this->comfirm_flag) return;
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
void SaveDataSelector::onSpacePressed(int idx)
{
    // インクリメント
    idx++;
    // 確認時
    if (this->comfirm_flag)
    {
        if(this->onSaveDataSelectCancelled)
        {
            this->onSaveDataSelectCancelled();
        }
        this->comfirm_flag = false;
        return;
    }
    // セーブorロード
    if (this->write_flag)
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
        this->comfirm_flag = true;
    }
    else
    {
        // ロード時
        if(this->existsSaveData[idx-1])
        {
            // ロード
            SoundManager::getInstance()->playSE(Resource::SE::LOAD);
            PlayerDataManager::getInstance()->setGameStart(idx);
            
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