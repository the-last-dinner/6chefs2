//
//  ModalLayerEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#include "Event/ModalLayerEvent.h"

#include "Event/EventFactory.h"
#include "Event/GameEventHelper.h"
#include "Event/EventScriptMember.h"

#include "Scenes/DungeonScene.h"

#include "Datas/Message/CharacterMessageData.h"
#include "Datas/Message/StoryMessageData.h"
#include "Datas/Message/SystemMessageData.h"

#include "Layers/Dungeon/DisplayImageLayer.h"
#include "Layers/Menu/SaveDataSelector.h"
#include "Layers/Message/CharacterMessagelayer.h"
#include "Layers/Message/StoryMessagelayer.h"
#include "Layers/Message/SystemMessagelayer.h"

#include "Managers/DungeonSceneManager.h"

#include "MapObjects/Character.h"
#include "MapObjects/Party.h"

#include "UI/Video/VideoPlayer.h"

#include "Models/PlayerData/LocalPlayerData.h"

#pragma mark ModalLayerEvent

// 初期化
bool ModalLayerEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    return true;
}

#pragma mark -
#pragma mark CharacterMessage

bool CharacterMessage::init(rapidjson::Value& json)
{
    if (!ModalLayerEvent::init(json)) return false;
    
    queue<CharacterMessageData*> datas {};
    
    string imageName = "";
    if (_eventHelper->hasMember(_json, member::IMG)) imageName = _json[member::IMG].GetString();
    
    //複数人での会話時
    if (_eventHelper->hasMember(_json, member::TALK)) {
        //会話人数の取得
        SizeType len { _json[member::TALK].Size() };
        
        for(SizeType i { 0 }; i < len; i++) {
            rapidjson::Value& chara { _json[member::TALK][i] };
            
            //ページ数を取得してページごとにプッシュ
            SizeType text_len { chara[member::TEXT].Size() };
            
            queue<string> pages {};
            
            for (SizeType j { 0 }; j < text_len; j++) {
                pages.push(chara[member::TEXT][j].GetString());
            }
            
            CharacterMessageData* data {CharacterMessageData::create(pages)};
            CC_SAFE_RETAIN(data);
            
            // キャラID
            if (_eventHelper->hasMember(chara, member::CHARA_ID)) {
                data->setCharaId(stoi(chara[member::CHARA_ID].GetString()));
            }
            
            // キャラ名
            string charaName {};
            
            if (_eventHelper->hasMember(chara, member::NAME)) {
                charaName = chara[member::NAME].GetString();
            } else {
                charaName = CsvDataManager::getInstance()->getCharacterData()->getName(data->getCharaId());
            }
            
            data->setCharaName(charaName);
            
            // 画像ID
            if (_eventHelper->hasMember(chara, member::IMG_ID)) data->setImgId(stoi(chara[member::IMG_ID].GetString()));
            
            // リアクション
            if (_eventHelper->hasMember(chara, member::OPTION)) data->setOption(CharacterMessageData::Option::REACTION);
            
            // ミニキャラ
            if (_eventHelper->hasMember(chara, member::MINI_CHARA)) data->setMiniChara(chara[member::MINI_CHARA].GetBool());
            
            // ボイス
            if (_eventHelper->hasMember(chara, member::VOICE)) data->setVoice(chara[member::VOICE].GetString());

            // 背景画像
            if (imageName != "") data->setImageName(imageName);
            
            datas.push(data);
        }
    }
    //一人で話すとき
    else
    {
        //ページ数を取得してページごとにプッシュ
        SizeType text_len { _json[member::TEXT].Size() };
        queue<string> pages {};
        
        for (SizeType j {0}; j < text_len; j++) {
            pages.push(_json[member::TEXT][j].GetString());
        }
        
        CharacterMessageData* data {CharacterMessageData::create(pages)};
        CC_SAFE_RETAIN(data);
        
        // キャラID
        if (_eventHelper->hasMember(_json, member::CHARA_ID)) data->setCharaId(stoi(_json[member::CHARA_ID].GetString()));
        
        // キャラ名
        string charaName {};
        
        if (_eventHelper->hasMember(_json, member::NAME)) {
            charaName = _json[member::NAME].GetString();
        } else {
            charaName = CsvDataManager::getInstance()->getCharacterData()->getName(data->getCharaId());
        }
        data->setCharaName(charaName);
        
        // 画像ID
        if (_eventHelper->hasMember(_json, member::IMG_ID)) data->setImgId(stoi(_json[member::IMG_ID].GetString()));
        
        // リアクション
        if (_eventHelper->hasMember(_json, member::OPTION)) data->setOption(CharacterMessageData::Option::REACTION);
        
        // ミニキャラ
        if (_eventHelper->hasMember(_json, member::MINI_CHARA)) data->setMiniChara(_json[member::MINI_CHARA].GetBool());
        
        // ボイス
        if (_eventHelper->hasMember(_json, member::VOICE)) data->setVoice(_json[member::VOICE].GetString());
        
        // 背景画像
        if (imageName != "") data->setImageName(imageName);

        datas.push(data);
    }
    
    // 最後に画像のみ表示するので入れる
    if(imageName != "") {
        queue<string> pages {};
        pages.push("");
        CharacterMessageData* data {CharacterMessageData::create(pages)};
        CC_SAFE_RETAIN(data);
        data->setImageName(imageName);
        data->setImageOnly(true);
        data->setCharaName("");
        datas.push(data);
    }
    
    _datas = datas;
    
    return true;
}

void CharacterMessage::run()
{
    DungeonSceneManager::getInstance()->getScene()->addChild(CharacterMessageLayer::create(_datas, [this]{this->setDone();}), Priority::CHARACTER_MESSAGE);
}

#pragma mark -
#pragma mark StoryMessage

bool StoryMessage::init(rapidjson::Value& json)
{
    if (!ModalLayerEvent::init(json)) return false;
    
    // タイトル
    if (_eventHelper->hasMember(_json, member::TITLE)) {
        _title = _json[member::TITLE].GetString();
    }
    
    queue<StoryMessageData*> datas;
    SizeType len { _json[member::TEXT].Size()};
    
    for (SizeType i {0}; i < len; i++) {
        StoryMessageData* data {StoryMessageData::create(_json[member::TEXT][i].GetString())};
        CC_SAFE_RETAIN(data);
        datas.push(data);
    }
    
    _datas = datas;
    
    return true;
}

void StoryMessage::run()
{
    DungeonSceneManager::getInstance()->getScene()->addChild(StoryMessageLayer::create(_title, _datas, [this]{this->setDone();}), Priority::STORY_MESSAGE);
}

#pragma mark -
#pragma mark SystemMessage

bool SystemMessage::init(rapidjson::Value& json)
{
    if (!ModalLayerEvent::init(json)) return false;
    
    queue<SystemMessageData*> datas;
    SizeType len { _json[member::TEXT].Size() };
    
    for (SizeType i {0}; i < len; i++) {
        SystemMessageData* data {SystemMessageData::create(_json[member::TEXT][i].GetString())};
        CC_SAFE_RETAIN(data);
        datas.push(data);
    }
    
    _datas = datas;
    
    return true;
}

void SystemMessage::run()
{
    DungeonSceneManager::getInstance()->getScene()->addChild(SystemMessageLayer::create(_datas, [this]{this->setDone();}), Priority::SYSTEM_MESSAGE);
}

#pragma mark -
#pragma mark DispImageEvent

bool DispImageEvent::init(rapidjson::Value& json)
{
    if (!ModalLayerEvent::init(json)) return false;
    
    // ファイル名
    if (!_eventHelper->hasMember(_json, member::FILE)) return false;
    _fileName = _json[member::FILE].GetString();
    
    // 表示時間
    if (_eventHelper->hasMember(_json, member::TIME)) _duration = _json[member::TIME].GetDouble();
    
    return true;
}

void DispImageEvent::run()
{
    DisplayImageLayer* layer { DisplayImageLayer::create(_fileName, _duration, [this]{this->setDone();}) };

    if (!layer) {
        this->setDone();
        
        return;
    }
    
    DungeonSceneManager::getInstance()->getScene()->addChild(layer, Priority::DISP_IMAGE_LAYER);
}

#pragma mark -
#pragma mark DisplaySaveMenu

bool DisplaySaveMenu::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    _saveMenu = SaveDataSelector::create(true);
    _saveMenu->setVisible(false);
    _saveMenu->onSaveDataSelectCancelled = CC_CALLBACK_0(DisplaySaveMenu::onExitedSaveMenu, this);
    
    return true;
}

void DisplaySaveMenu::run()
{
    LocalPlayerData* localPlayerData { PlayerDataManager::getInstance()->getLocalData() };
    Location location { localPlayerData->getLocation() };
    
    if (_eventHelper->hasMember(_json, member::MAP_ID)) {
        location.map_id = stoi(_json[member::MAP_ID].GetString());
    }
    
    if (_eventHelper->hasMember(_json, member::X)) {
        location.x = _json[member::X].GetInt();
    }
    
    if (_eventHelper->hasMember(_json, member::Y)) {
        location.y = _json[member::Y].GetInt();
    }
        
    if (_eventHelper->hasMember(_json, member::DIRECTION)) {
        location.direction = _eventHelper->getDirection(_json);
    }
    
    if (_eventHelper->hasMember(_json, member::EVENT_ID)) {
        localPlayerData->setInitEventId(_json[member::EVENT_ID].GetString());
    }
    
    // 主人公一行の位置を登録
    vector<CharacterData> members { DungeonSceneManager::getInstance()->getParty()->getMembersData() };
    int membersSize { static_cast<int>(members.size()) };
    for (int i = 0; i < membersSize; i++) {
        members[i].location = location;
    }
    localPlayerData->setLocation(members);
    
    this->display();
}

void DisplaySaveMenu::display()
{
    if (!_saveMenu) {
        this->setDone();
        return;
    }
    
    DungeonSceneManager::getInstance()->getScene()->addChild(_saveMenu, Priority::SELECT_LAYER);
    SoundManager::getInstance()->playSE(Resource::SE::TITLE_ENTER);
    _saveMenu->show();
}

void DisplaySaveMenu::onExitedSaveMenu()
{
    SoundManager::getInstance()->playSE(Resource::SE::BACK);
    _saveMenu->runAction(
        Sequence::createWithTwoActions(
            CallFunc::create([this](){this->_saveMenu->hide();}),
            DelayTime::create(0.3f)
        )
    );
    PlayerDataManager::getInstance()->getLocalData()->removeInitEventId();
    this->setDone();
}

#pragma mark -
#pragma mark PlayVideoEvent

bool PlayVideoEvent::init(rapidjson::Value& json)
{
    if (!ModalLayerEvent::init(json)) return false;
    
    // ファイル名
    if (!_eventHelper->hasMember(_json, member::FILE)) return false;
    
    _fileName = _json[member::FILE].GetString();
    
    // スキップ可能か
    if (_eventHelper->hasMember(_json, member::SKIP))
        _skip = _json[member::SKIP].GetBool();
    
    return true;
}

void PlayVideoEvent::run()
{
    VideoPlayer* layer { VideoPlayer::create(_fileName, _skip, [this]{this->setDone();}) };
    
    if (!layer) {
        this->setDone();
        
        return;
    }
    
    DungeonSceneManager::getInstance()->getScene()->addChild(layer, Priority::VIDEO_LAYER);
}
