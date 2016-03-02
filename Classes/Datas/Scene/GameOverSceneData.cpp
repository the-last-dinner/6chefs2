//
//  GameOverSceneData.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#include "Datas/Scene/GameOverSceneData.h"

// コンストラクタ
GameOverSceneData::GameOverSceneData() {FUNCLOG};

// デストラクタ
GameOverSceneData::~GameOverSceneData() {FUNCLOG};

// 初期化
bool GameOverSceneData::init()
{
    this->textureFilePaths =
    {
        Resource::SpriteFrame::BLOOD,
    };
    
    this->soundFilePaths = {};
    
    return true;
}