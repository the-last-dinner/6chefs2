//
//  TextureManager.h
//  LastSupper
//
//  Created by Kohei on 2015/07/20.
//
//

#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include "define.h"

class TextureManager{
	// クラスメソッド
public:
	static TextureManager* getInstance();
	static void destory();
	
	// シングルトンであるためにprivateに
private:
	TextureManager();												// コンストラクタ
	TextureManager(const TextureManager& other);					// デストラクタ
	TextureManager& operator = (const TextureManager& other);		// 代入演算子
	~TextureManager();												// デストラクタ
	
public:
	void preloadTexture(const string& filePath, const function<void()> callback);
	void unloadAllTectures();
	
	// インスタンス変数
private:
	vector<string> texturePaths;
};

#endif // __TEXTURE_MANAGER_H__
