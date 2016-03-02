//
//  EndingSceneData.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/03.
//
//

#include "Datas/Scene/EndingSceneData.h"

// コンストラクタ
EndingSceneData::EndingSceneData() {FUNCLOG};

// デストラクタ
EndingSceneData::~EndingSceneData() {FUNCLOG};

// 初期化
bool EndingSceneData::init()
{
    this->textureFilePaths = {};
    this->soundFilePaths = {};
    
    return true;
}