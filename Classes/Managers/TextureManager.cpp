//
//  TextureManager.cpp
//  LastSupper
//
//  Created by Kohei on 2015/07/20.
//
//

#include "Managers/TextureManager.h"

// 唯一のインスタンスを初期化
static TextureManager* _instance = nullptr;

// 唯一のインスタンスを取得
TextureManager* TextureManager::getInstance()
{
	if(!_instance) _instance = new TextureManager();
	return _instance;
}

// インスタンスを破棄
void TextureManager::destory()
{
	delete _instance;
	return;
}

// コンストラクタ
TextureManager::TextureManager()
{FUNCLOG}

// デストラクタ
TextureManager::~TextureManager()
{FUNCLOG}

// 指定パスのテクスチャをプリロード
void TextureManager::preloadTexture(const string& filePath, const function<void()> callback)
{
	Director::getInstance()->getTextureCache()->addImageAsync(filePath + ".png",
															  [=](Texture2D* loaded_texture){
																  SpriteFrameCache::getInstance()->addSpriteFramesWithFile(filePath + ".plist", loaded_texture);
																  this->texturePaths.push_back(filePath);
																  callback();
															  });
	return;
}

// すべてのテクスチャをアンロード
void TextureManager::unloadAllTectures()
{
	for(string filePath : this->texturePaths)
	{
		SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(filePath + ".plist");
		Director::getInstance()->getTextureCache()->removeTextureForKey(filePath + ".png");
	}
	this->texturePaths.clear();
}