//
//  DungeonSceneEventHandler.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/10/02.
//
//

#include "Scenes/EventHandler/DungeonSceneEventHandler.h"

#include "Layers/Dungeon/TiledMapLayer.h"

#include "MapObjects/MapObjectList.h"

#include "Scenes/DungeonScene.h"
#include "Scenes/GameOverScene.h"

#include "Tasks/PlayerControlTask.h"

// コンストラクタ
DungeonSceneEventHandler::DungeonSceneEventHandler() {}

// デストラクタ
DungeonSceneEventHandler::~DungeonSceneEventHandler() {}

// 初期化
bool DungeonSceneEventHandler::init(DungeonScene* scene)
{
    _scene = scene;
    
    return true;
}

// 主人公のHPがなくなった時
void DungeonSceneEventHandler::onLostMainCharacterHP()
{
    if (ConfigDataManager::getInstance()->getDebugConfigData()->getBoolValue(DebugConfigData::INVINCIBLE_MODE)) return;
    _scene->onExitDungeon();
    Director::getInstance()->replaceScene(GameOverScene::create(GameOverScene::Type::BLOOD));
}

// バトル開始時
void DungeonSceneEventHandler::onBattleStart()
{
    _scene->_playerControlTask->onBattleStart();
    _scene->_mapLayer->getMapObjectList()->onBattleStart();
}

// バトル終了時
void DungeonSceneEventHandler::onBattleFinished()
{
    _scene->_mapLayer->getMapObjectList()->onBattleFinished();
    _scene->_playerControlTask->onBattleFinished();
}
