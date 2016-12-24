//
//  SoundManager.cpp
//  LastSupper
//
//  Created by Kohei on 2015/06/22.
//
//

#include "Managers/SoundManager.h"
#include "Managers/PlayerDataManager.h"
#include "Models/PlayerData/GlobalPlayerData.h"
#include "Utils/AssertUtils.h"

using namespace cocos2d::experimental;

// 定数
const map<string, float> SoundManager::VOLUME_CONFIG
{
    {Resource::SE::CURSOR_MOVE, 0.3f},
    {Resource::SE::BACK, 0.3f},
    {Resource::SE::TITLE_ENTER, 0.3f},
    {Resource::SE::GAME_START, 0.3f},
    {Resource::SE::LOAD, 0.3f},
};

// 唯一のインスタンスを初期化
static SoundManager* _instance = nullptr;

// 唯一のインスタンスを取得
SoundManager* SoundManager::getInstance()
{
	if(!_instance) _instance = new SoundManager();
	return _instance;
}

// インスタンスを破棄
void SoundManager::destory()
{
	delete _instance;
    _instance = nullptr;
}

// コンストラクタ
SoundManager::SoundManager()
{FUNCLOG}

// デストラクタ
SoundManager::~SoundManager()
{FUNCLOG}

// SEを再生
void SoundManager::playSE(const string& fileName, float volume)
{
    if(VOLUME_CONFIG.count(fileName) != 0) volume *= VOLUME_CONFIG.at(fileName);
    
    string filePath = Resource::SE::BASE_PATH + fileName;
    
    if (FileUtils::getInstance()->fullPathForFilename(filePath) == "") {
        LastSupper::AssertUtils::warningAssert("PlaySE ERROR\n" + fileName + " is missing.");
    }
    
    int seId { AudioEngine::play2d(filePath, false, volume * PlayerDataManager::getInstance()->getGlobalData()->getSeVolume()) };
    AudioEngine::setFinishCallback(seId, CC_CALLBACK_2(SoundManager::onSEFinished, this));
    
    mtx.lock();
    
    this->seIdMap.insert({seId, fileName});
    
    mtx.unlock();
}

// voiceを再生
void SoundManager::playVoice(const string &fileName, float volume, const std::function<void(int,const std::string&)>& onVoiceFinished)
{
    if (VOLUME_CONFIG.count(fileName) != 0) volume *= VOLUME_CONFIG.at(fileName);
    
    string filePath = Resource::VOICE::BASE_PATH + fileName;
    
    if (FileUtils::getInstance()->fullPathForFilename(filePath) == "") {
        LastSupper::AssertUtils::warningAssert("PlayVoice ERROR\n" + fileName + " is missing.");
    }
    
    int voiceId { AudioEngine::play2d(filePath, false, volume * PlayerDataManager::getInstance()->getGlobalData()->getVoiceVolume())};
    AudioEngine::setFinishCallback(voiceId, onVoiceFinished);
    
    mtx.lock();
    
    this->seIdMap.insert({voiceId, fileName});
    
    mtx.unlock();
}
void SoundManager::playVoice(const string &filePath, float volume)
{
    this->playVoice(filePath, volume, nullptr);
}

// BGMを再生
void SoundManager::playBGM(const string& fileName, bool loop, float volume)
{
    if(VOLUME_CONFIG.count(fileName) != 0) volume *= VOLUME_CONFIG.at(fileName);
    
    string filePath = Resource::BGM::BASE_PATH + fileName;
    
    if (FileUtils::getInstance()->fullPathForFilename(filePath) == "") {
        LastSupper::AssertUtils::warningAssert("PlayBGM ERROR\n" + fileName + " is missing.");
    }
    
    int BGMId { AudioEngine::play2d(filePath, loop, volume * PlayerDataManager::getInstance()->getGlobalData()->getBgmVolume()) };
    AudioEngine::setFinishCallback(BGMId, CC_CALLBACK_2(SoundManager::onBGMFinished, this));
    
    mtx.lock();
    
    this->bgmIdMap.insert({BGMId, fileName});
    
    mtx.unlock();
}

// BGMの音量を変更
void SoundManager::changeVolume(const string& fileName, float volume)
{
    if(VOLUME_CONFIG.count(fileName) != 0) volume *= VOLUME_CONFIG.at(fileName);
    int BGMId = -1;
    for(pair<int, string> idToFilename : this->bgmIdMap)
    {
        if(idToFilename.second == fileName)
        {
            BGMId = idToFilename.first;
            break;
        }
    }
    if(BGMId != -1) AudioEngine::setVolume(BGMId, volume * PlayerDataManager::getInstance()->getGlobalData()->getBgmVolume());
}

// SE再生終了時
void SoundManager::onSEFinished(int seId, const string& filename)
{
}

// BGM再生終了時
void SoundManager::onBGMFinished(int bgmId, const string& filename)
{
}

// BGMをファイル名指定で停止
void SoundManager::stopBGM(const string& filePath)
{
    for(pair<int, string> idToPath : this->bgmIdMap)
    {
        if(idToPath.second == filePath) AudioEngine::stop(idToPath.first);
    }
}

// BGM全て停止
void SoundManager::stopBGMAll()
{
    if(this->bgmIdMap.empty()) return;
    
    for(pair<int, string> idToPath : this->bgmIdMap)
    {
        AudioEngine::stop(idToPath.first);
    }
}

// 指定ファイルが再生中かどうか
bool SoundManager::isPlaying(const string& filePath) const
{
    for(pair<int, string> idToFilename : this->bgmIdMap)
    {
        if(idToFilename.second != filePath) continue;
        if(AudioEngine::AudioState::PLAYING != AudioEngine::getState(idToFilename.first)) continue;
        
        return true;
    }
    
    for(pair<int, string> idToFilename : this->seIdMap)
    {
        if(idToFilename.second != filePath) continue;
        if(AudioEngine::AudioState::PLAYING != AudioEngine::getState(idToFilename.first)) continue;
        
        return true;
    }
    
    return false;
}

// 初期化中か
bool SoundManager::isInitializing(const string& filePath) const
{
    for(pair<int, string> idToFilename : this->bgmIdMap)
    {
        if(idToFilename.second != filePath) continue;
        if(AudioEngine::AudioState::INITIALZING != AudioEngine::getState(idToFilename.first)) continue;
        
        return true;
    }
    
    for(pair<int, string> idToFilename : this->seIdMap)
    {
        if(idToFilename.second != filePath) continue;
        if(AudioEngine::AudioState::INITIALZING != AudioEngine::getState(idToFilename.first)) continue;
        
        return true;
    }
    
    return false;
}

// 音声ファイルをプリロード
void SoundManager::preloadSound(const string& filePath, function<void(bool)> callback)
{
    AudioEngine::preload(filePath, callback);
}

// 音声をアンロード
void SoundManager::unloadAllSounds()
{
    mtx.lock();
    
    for(pair<int, string> idToFilename : this->seIdMap)
    {
        if(this->isPlaying(idToFilename.second)) continue;
        
        AudioEngine::stop(idToFilename.first);
        AudioEngine::uncache(Resource::SE::BASE_PATH + idToFilename.second);
    }
    
    for(pair<int, string> idToFilename : this->bgmIdMap)
    {
        if(this->isPlaying(idToFilename.second)) continue;
        
        AudioEngine::stop(idToFilename.first);
        AudioEngine::uncache(Resource::BGM::BASE_PATH + idToFilename.second);
    }
    
    mtx.unlock();
}
