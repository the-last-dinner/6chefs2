//
//  DungeonSceneData.cpp
//  LastSupper
//
//  Created by Kohei on 2015/06/23.
//
//

#include "Datas/Scene/DungeonSceneData.h"

#include "Event/EventScript.h"

// コンストラクタ
DungeonSceneData::DungeonSceneData() {FUNCLOG}

// デストラクタ
DungeonSceneData::~DungeonSceneData() {FUNCLOG}

// 初期化
bool DungeonSceneData::init(const Location& location)
{
    // ロケーション格納
    this->location = location;
	
	return true;
}

// changeMap時に渡されるEventIDを設定
void DungeonSceneData::setInitialEventId(const int eventId)
{
    this->initEventId = eventId;
}

// changeMap時に渡されるEventIDを取得
int DungeonSceneData::getInitialEventId() const
{
    return this->initEventId;
}

// 場所を取得
Location DungeonSceneData::getInitialLocation() const
{
    return this->location;
}