//
//  ModalLayerEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/25.
//
//

#include "Event/ModalLayerEvent.h"

#include "Event/EventFactory.h"
#include "Event/EventScriptValidator.h"
#include "Event/EventScriptMember.h"

#include "Scenes/DungeonScene.h"

#include "Datas/Message/CharacterMessageData.h"
#include "Datas/Message/StoryMessageData.h"
#include "Datas/Message/SystemMessageData.h"

#include "Layers/Dungeon/DisplayImageLayer.h"
#include "Layers/Message/CharacterMessagelayer.h"
#include "Layers/Message/StoryMessagelayer.h"
#include "Layers/Message/SystemMessagelayer.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark ModalLayerEvent

// 初期化
bool ModalLayerEvent::init()
{
    if(!GameEvent::init()) return false;
    
    return true;
}

#pragma mark -
#pragma mark CharacterMessage

bool CharacterMessage::init(rapidjson::Value& json)
{
    if(!ModalLayerEvent::init()) return false;
    
    queue<CharacterMessageData*> datas {};
    
    //複数人での会話時
    if(this->validator->hasMember(json, member::TALK))
    {
        //会話人数の取得
        SizeType len {json[member::TALK].Size()};
        
        for(SizeType i { 0 }; i < len; i++)
        {
            rapidjson::Value& chara { json[member::TALK][i] };
            
            //ページ数を取得してページごとにプッシュ
            SizeType text_len {chara[member::TEXT].Size()};
            
            queue<string> pages {};
            
            for(SizeType j { 0 }; j < text_len; j++)
            {
                pages.push(chara[member::TEXT][j].GetString());
            }
            
            CharacterMessageData* data {CharacterMessageData::create(pages)};
            CC_SAFE_RETAIN(data);
            
            // キャラID
            if (this->validator->hasMember(chara, member::CHARA_ID))
            {
                data->setCharaId(stoi(chara[member::CHARA_ID].GetString()));
            }
            
            // キャラ名
            string charaName {};
            
            if(this->validator->hasMember(chara, member::NAME))
            {
                charaName = chara[member::NAME].GetString();
            }
            else
            {
                charaName = CsvDataManager::getInstance()->getCharacterData()->getName(data->getCharaId());
            }
            
            data->setCharaName(charaName);
            
            // 画像ID
            if(this->validator->hasMember(chara, member::IMG_ID)) data->setImgId(stoi(chara[member::IMG_ID].GetString()));
            
            // リアクション
            if(this->validator->hasMember(chara, member::OPTION)) data->setOption(CharacterMessageData::Option::REACTION);
            
            datas.push(data);
        }
    }
    //一人で話すとき
    else
    {
        //ページ数を取得してページごとにプッシュ
        SizeType text_len {json[member::TEXT].Size()};
        queue<string> pages {};
        
        for(SizeType j {0}; j < text_len; j++)
        {
            pages.push(json[member::TEXT][j].GetString());
        }
        
        CharacterMessageData* data {CharacterMessageData::create(pages)};
        CC_SAFE_RETAIN(data);
        
        // キャラID
        if(this->validator->hasMember(json, member::CHARA_ID)) data->setCharaId(stoi(json[member::CHARA_ID].GetString()));
        
        // キャラ名
        string charaName {};
        
        if(this->validator->hasMember(json, member::NAME))
        {
            charaName = json[member::NAME].GetString();
        }
        else
        {
            charaName = CsvDataManager::getInstance()->getCharacterData()->getName(data->getCharaId());
        }
        data->setCharaName(charaName);
        
        // 画像ID
        if(this->validator->hasMember(json, member::IMG_ID)) data->setImgId(stoi(json[member::IMG_ID].GetString()));
        
        // リアクション
        if(this->validator->hasMember(json, member::OPTION)) data->setOption(CharacterMessageData::Option::REACTION);
        
        datas.push(data);
    }
    
    this->datas = datas;
    
    return true;
}

void CharacterMessage::run()
{
    DungeonSceneManager::getInstance()->getScene()->addChild(CharacterMessageLayer::create(this->datas, [this]{this->setDone();}), Priority::CHARACTER_MESSAGE);
}

#pragma mark -
#pragma mark StoryMessage

bool StoryMessage::init(rapidjson::Value& json)
{
    if(!ModalLayerEvent::init()) return false;
    
    // タイトル
    if(this->validator->hasMember(json, member::TITLE))
    {
        this->title = json[member::TITLE].GetString();
    }
    
    queue<StoryMessageData*> datas;
    SizeType len {json[member::TEXT].Size()};
    
    for (SizeType i {0}; i < len; i++)
    {
        StoryMessageData* data {StoryMessageData::create(json[member::TEXT][i].GetString())};
        CC_SAFE_RETAIN(data);
        datas.push(data);
    }
    
    this->datas = datas;
    
    return true;
}

void StoryMessage::run()
{
    DungeonSceneManager::getInstance()->getScene()->addChild(StoryMessageLayer::create(this->title, this->datas, [this]{this->setDone();}), Priority::STORY_MESSAGE);
}

#pragma mark -
#pragma mark SystemMessage

bool SystemMessage::init(rapidjson::Value& json)
{
    if(!ModalLayerEvent::init()) return false;
    
    queue<SystemMessageData*> datas;
    SizeType len {json[member::TEXT].Size()};
    
    for (SizeType i {0}; i < len; i++)
    {
        SystemMessageData* data {SystemMessageData::create(json[member::TEXT][i].GetString())};
        CC_SAFE_RETAIN(data);
        datas.push(data);
    }
    
    this->datas = datas;
    
    return true;
}

void SystemMessage::run()
{
    DungeonSceneManager::getInstance()->getScene()->addChild(SystemMessageLayer::create(this->datas, [this]{this->setDone();}), Priority::SYSTEM_MESSAGE);
}

#pragma mark -
#pragma mark DispImageEvent

bool DispImageEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    // ファイル名
    if(!this->validator->hasMember(json, member::FILE)) return false;
    this->fileName = json[member::FILE].GetString();
    
    // 表示時間
    if(this->validator->hasMember(json, member::TIME)) this->duration = json[member::TIME].GetDouble();
    
    return true;
}

void DispImageEvent::run()
{
    DisplayImageLayer* layer { DisplayImageLayer::create(this->fileName, this->duration, [this]{this->setDone();}) };

    if(!layer)
    {
        this->setDone();
        
        return;
    }
    
    DungeonSceneManager::getInstance()->getScene()->addChild(layer, Priority::DISP_IMAGE_LAYER);
}

