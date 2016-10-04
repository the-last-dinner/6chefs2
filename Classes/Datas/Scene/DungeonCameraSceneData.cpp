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
    _targetId = objId;
}

// ターゲットのIDを取得
int DungeonCameraSceneData::getTargetId() const
{
    return _targetId;
}

// イベントJsonを設定
void DungeonCameraSceneData::setEventJson(rapidjson::Value& json)
{
    _eventJson = json;
}

// イベントJsonを取得
rapidjson::Value& DungeonCameraSceneData::getEventJson()
{
    return _eventJson;
}
