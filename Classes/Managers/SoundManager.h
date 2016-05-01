//
//  SoundManager.h
//  LastSupper
//
//  Created by Kohei on 2015/06/22.
//
//

#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "define.h"

class SoundManager
{
// 定数
private:
    static const map<string, float> VOLUME_CONFIG;
    
// クラスメソッド
public:
	static SoundManager* getInstance();
	static void destory();
	
// シングルトンであるためにprivateに
private:
	SoundManager();												// コンストラクタ
	SoundManager(const SoundManager& other);					// デストラクタ
	SoundManager& operator = (const SoundManager& other);		// 代入演算子
	~SoundManager();											// デストラクタ

public:
    void playSE(const string& filePath, float volume = 1.f);
    void playBGM(const string& filePath, bool loop = true, float volume = 1.f);
    
    void changeVolume(const string& filePath, float volume = 1.f);
    
    void stopBGM(const string& filePath);
    void stopBGMAll();
    
    void onSEFinished(int seId, const string& filename);
    void onBGMFinished(int bgmId, const string& filename);
    
    bool isPlaying(const string& filePath) const;
    bool isInitializing(const string& filePath) const;
    
    void preloadSound(const string& filePath, function<void(bool)> callback);
    
	void unloadAllSounds();
	
// インスタンス変数
private:
    map<int, string> preloadMap {};
    map<int, string> seIdMap {};
    map<int, string> bgmIdMap {};
    std::mutex mtx {};
};

#endif // __SOUND_MANAGER_H__
