//
//  SearchState.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/20.
//
//

#include "Tasks/PlayerControlState/SearchState.h"

#include "MapObjects/Character.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/Party.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
SearchState::SearchState() { FUNCLOG }

// デストラクタ
SearchState::~SearchState() { FUNCLOG }

// 初期化
bool SearchState::init()
{
    if (!PlayerControlState::init()) return false;
    
    return true;
}

// 決定キーが押された時
void SearchState::onEnterKeyPressed(Party* party)
{
    MapObjectList* objectList { DungeonSceneManager::getInstance()->getMapObjectList() };
    Character* mainCharacter { party->getMainCharacter() };
    
    Vector<MapObject*> objs { objectList->getMapObjects(mainCharacter, { mainCharacter->getDirection() }, Trigger::SEARCH) };
    
    // 同座標にあるイベントを全て発動
    Point objPosition { Point::ZERO };
    for (MapObject* obj : objs) {
        if (obj && obj->getTrigger() == Trigger::SEARCH && (objPosition == Point::ZERO || obj->getPosition() == objPosition) && !obj->isMoving()) {
            objPosition = obj->getPosition();
            obj->onSearched(mainCharacter);
            DungeonSceneManager::getInstance()->runEvent(obj->getEventId());
        }
    }
}
