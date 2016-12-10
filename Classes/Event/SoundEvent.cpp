//
//  SoundEvent.cpp
//  LastSupper
//
//  Created by Kohei on 2015/10/25.
//
//

#include "SoundEvent.h"

#include "Event/EventScriptMember.h"

#include "Event/GameEventHelper.h"

#pragma mark PlayBGMEvent

bool PlayBGMEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // ファイル名。なければ生成しない
    if (!_eventHelper->hasMember(_json, member::FILE)) return false;
    
    _fileName = _json[member::FILE].GetString();
    
    // 音量
    if (_eventHelper->hasMember(_json, member::VOLUME)) _volume = _json[member::VOLUME].GetDouble();
    
    return true;
}

void PlayBGMEvent::run()
{
    SoundManager::getInstance()->playBGM(_fileName, true, _volume);
    PlayerDataManager::getInstance()->getLocalData()->setBgm(_fileName);
    this->setDone();
}

#pragma mark -
#pragma mark StopBGMEvent

bool StopBGMEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // ファイル名
    if (_eventHelper->hasMember(_json, member::FILE)) _fileName = _json[member::FILE].GetString();
    
    return true;
}

void StopBGMEvent::run()
{
    if (_fileName != "") {
        SoundManager::getInstance()->stopBGM(_fileName);
        PlayerDataManager::getInstance()->getLocalData()->removeBgm(_fileName);
    } else {
        SoundManager::getInstance()->stopBGMAll();
        PlayerDataManager::getInstance()->getLocalData()->removeBgmAll();
    }
    
    this->setDone();
}

#pragma mark -
#pragma mark ChangeVolumeEvent

bool ChangeVolumeEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // ファイル名。なければ生成しない
    if (!_eventHelper->hasMember(_json, member::FILE)) return false;
    
    _fileName = _json[member::FILE].GetString();
    
    // 音量
    if (_eventHelper->hasMember(_json, member::VOLUME)) _volume = _json[member::VOLUME].GetDouble();
    
    return true;
}

void ChangeVolumeEvent::run()
{
    SoundManager::getInstance()->changeVolume(_fileName, _volume);
    this->setDone();
}

#pragma mark -
#pragma mark PlaySEEvent

bool PlaySEEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    // ファイル名。なければ生成しない
    if (!_eventHelper->hasMember(_json, member::FILE)) return false;
    
    _fileName = _json[member::FILE].GetString();
    
    // 音量
    if (_eventHelper->hasMember(_json, member::VOLUME)) _volume = _json[member::VOLUME].GetDouble();
    
    return true;
}

void PlaySEEvent::run()
{
    SoundManager::getInstance()->playSE(_fileName, _volume);
    this->setDone();
}

#pragma mark -
#pragma mark PlayVoiceEvent

bool PlayVoiceEvent::init(rapidjson::Value& json)
{
    if (!GameEvent::init(json)) return false;
    
    if (!_eventHelper->hasMember(_json, member::FILE)) return false;
    
    _fileName = _json[member::FILE].GetString();
    
    // 音量
    if (_eventHelper->hasMember(_json, member::VOLUME)) _volume = _json[member::VOLUME].GetDouble();
    
    return true;
}

void PlayVoiceEvent::run()
{
    SoundManager::getInstance()->playVoice(_fileName, _volume);
    this->setDone();
}
