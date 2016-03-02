//
//  DungeonCameraSceneData.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/28.
//
//

#include "Datas/Scene/DungeonCameraSceneData.h"

// コンストラクタ
DungeonCameraSceneData::DungeonCameraSceneData() { FUNCLOG };

// デストラクタ
DungeonCameraSceneData::~DungeonCameraSceneData() { FUNCLOG };

// 初期化
bool DungeonCameraSceneData::init(const Location& location)
{
    if(!DungeonSceneData::init(location)) return false;
    
    return true;
}

// ターゲットのIDを設定
void DungeonCameraSceneData::setTargetId(const int objId)
{
    this->targetId = objId;
}

// ターゲットのIDを取得
int DungeonCameraSceneData::getTargetId() const
{
    return this->targetId;
}