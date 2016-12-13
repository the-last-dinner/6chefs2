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

#include "MapObjects/Status/Stamina.h"

#include "Tasks/PlayerControlTask.h"

// コンストラクタ
SearchState::SearchState() { FUNCLOG }

// デストラクタ
SearchState::~SearchState() { FUNCLOG }

// 初期化
bool SearchState::init(PlayerControlTask* task)
{
    if (!PlayerControlState::init(task)) return false;
    return true;
}

#pragma mark -
#pragma mark Interface

// 振り向き
void SearchState::turn(Party* party, const Direction& direction, bool isDashKeyPressed)
{
    Character* mainCharacter { party->getMainCharacter() };
    
    // 主人公の向きを変更
    mainCharacter->setDirection(direction);
    
    // 主人公が移動中でなければ
    if (!mainCharacter->isMoving()) {
        // 一定時間後に歩行開始
        if (!_task->isScheduled(PlayerControlTask::START_WALKING_SCHEDULE_KEY)) _task->scheduleOnce([this, party](float _) {
            _task->move(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
        }, 0.0f, PlayerControlTask::START_WALKING_SCHEDULE_KEY);
    }
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

// 移動
void SearchState::move(Party* party, const vector<Direction>& directions, bool isDashKeyPressed)
{
    Character* mainCharacter { party->getMainCharacter() };
    
    // ダッシュキーが押されていたら、速度の倍率をあげる
    bool dash { mainCharacter->isRunnable() ? isDashKeyPressed : false };
    
    // 入力から、使う方向の個数を決める
    int directionCount { (directions.size() == 2 && !Direction::getVec2({directions.front(), directions.back()}).isZero()) ? 2 : 1 };
    
    vector<Direction> moveDirections {};
    for (int i { 0 }; i < directions.size(); i++) {
        if (directions.size() - directionCount > i) continue;
        moveDirections.push_back(directions.at(i));
    }
    
    // Trigger::WILLを持つオブジェクトを検索して実行
    Rect gridRect { mainCharacter->getGridCollisionRect() };
    gridRect.origin += Direction::getGridVec2(moveDirections);
    for (MapObject* other : DungeonSceneManager::getInstance()->getMapObjectList()->getMapObjects(mainCharacter, moveDirections, Trigger::WILL)) {
        DungeonSceneManager::getInstance()->runEvent(other->getEventId());
    }
    
    Stamina* stamina { DungeonSceneManager::getInstance()->getStamina() };
    
    party->move(moveDirections, dash ? PlayerControlTask::DASH_SPEED_RATIO : 1.f, [this, party, stamina, dash](bool moved) {
        if (!moved) return;
        
        stamina->setDecreasing(false);
        _task->onPartyMovedOneGrid(party, dash);
    });
}
