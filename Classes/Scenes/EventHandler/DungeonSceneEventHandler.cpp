//
//  DungeonSceneEventHandler.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/10/02.
//
//

#include "Scenes/EventHandler/DungeonSceneEventHandler.h"

#include "Scenes/DungeonScene.h"
#include "Scenes/GameOverScene.h"

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
