//
//  PlayerControlTask.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/30.
//
//

#include "Tasks/PlayerControlTask.h"

#include "MapObjects/Character.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/Party.h"
#include "MapObjects/TerrainObject/TerrainObject.h"

#include "Models/Stamina.h"

#include "Managers/DungeonSceneManager.h"

// 定数
const string PlayerControlTask::START_WALKING_SCHEDULE_KEY { "start_walking" };
const float PlayerControlTask::DASH_SPEED_RATIO {2.0f};

// コンストラクタ
PlayerControlTask::PlayerControlTask(){FUNCLOG}

// デストラクタ
PlayerControlTask::~PlayerControlTask(){FUNCLOG}

// 初期化
bool PlayerControlTask::init()
{
    if(!GameTask::init()) return false;
    
    DungeonSceneManager::getInstance()->getStamina()->onIncreasedMax = CC_CALLBACK_0(PlayerControlTask::onStaminaIncreasedMax, this);
    
    return true;
}

// 向きを変える
void PlayerControlTask::turn(const Key& key, Party* party)
{
    if(!this->isControlEnabled()) return;
    
    Direction direction { MapUtils::keyToDirection(key) };
    Character* mainCharacter {party->getMainCharacter()};
    
    // 主人公の向きを変更
    mainCharacter->setDirection(direction);
    
    // 主人公が移動中でなければ
    if(!mainCharacter->isMoving())
    {
        // 一定時間後に歩行開始
        if(!this->isScheduled(START_WALKING_SCHEDULE_KEY)) this->scheduleOnce([this, party](float _)
        {
            this->walk(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
        }, 0.0f, START_WALKING_SCHEDULE_KEY);
    }
}

// 目の前を調べる
void PlayerControlTask::search(Party* party)
{
    if(!this->isControlEnabled()) return;
    
    MapObjectList* objectList {DungeonSceneManager::getInstance()->getMapObjectList()};
    Character* mainCharacter {party->getMainCharacter()};
    
    Vector<MapObject*> objs { objectList->getMapObjects(mainCharacter->getCollisionRect(mainCharacter->getDirection()))};
    
    // 同座標にあるイベントを全て発動
    Point objPosition {Point::ZERO};
    for(MapObject* obj : objs)
    {
        if(obj && obj->getTrigger() == Trigger::SEARCH && (objPosition == Point::ZERO || obj->getPosition() == objPosition) && !obj->isMoving())
        {
            objPosition = obj->getPosition();
            obj->onSearched(mainCharacter);
            DungeonSceneManager::getInstance()->runEvent(obj->getEventId());
        }
    }
}

// 歩行中、あたり判定を行い次に向かう位置を決定する
void PlayerControlTask::walk(const vector<Key>& keys, Party* party)
{
    if(keys.empty() || !this->isControlEnabled() || party->getMainCharacter()->isMoving()) return;
    
    vector<Direction> directions { MapUtils::keyToDirection(keys) };
    
    Character* mainCharacter {party->getMainCharacter()};
    
    // 一番最近押したキーの方向に主人公を向ける
    mainCharacter->setDirection(directions.back());
    
    // 移動前の地形オブジェクトを取得
    TerrainObject* terrain { mainCharacter->getTerrain() };
    
    // ダッシュキーが押されていたら、速度の倍率をあげる
    bool dash { terrain->canDash() ? DungeonSceneManager::getInstance()->isPressed(Key::DASH) : false };
    
    // 入力から、使う方向の個数を決める
    int directionCount {(directions.size() == 2 && directions.back() != directions.at(directions.size() - 2) && etoi(directions.back()) + etoi(directions.at(directions.size() - 2)) != 3)?etoi(directions.size()):1};
    
    vector<Direction> moveDirections {};
    for(int i { 0 }; i < directions.size(); i++)
    {
        if(directions.size() - directionCount > i) continue;
        moveDirections.push_back(directions.at(i));
    }
    
    // Trigger::WILLを持つオブジェクトを検索して実行
    Rect gridRect {mainCharacter->getGridRect()};
    gridRect.origin += MapUtils::directionsToMapVector(moveDirections);
    DungeonSceneManager::getInstance()->runEvent(DungeonSceneManager::getInstance()->getMapObjectList()->getEventIdsByGridRect(gridRect, Trigger::WILL));
    
    Stamina* stamina {DungeonSceneManager::getInstance()->getStamina()};
    
    if(!party->move(moveDirections, dash ? DASH_SPEED_RATIO : 1.f, [this, party, stamina]{stamina->setDecreasing(false); this->onPartyMovedOneGrid(party);})) return;
    
    // 地形から、スタミナ減少の倍率を取得しセット
    stamina->setStepRatio(terrain->getStaminaConsumptionRate());
    
    // 敵出現中かつ、ダッシュ中ならスタミナを減少モードにする
    stamina->setDecreasing(DungeonSceneManager::getInstance()->existsEnemy() && (dash || terrain->consumeStaminaWalking()));
    
    // スタミナが危険値まで下がっていたら、bgmを再生
    if(stamina->isWarn() && !SoundManager::getInstance()->isPlaying(Resource::BGM::TIRED)) SoundManager::getInstance()->playBGM(Resource::BGM::TIRED, true, 2.0f);
    
    Vector<MapObject*> objs { DungeonSceneManager::getInstance()->getMapObjectList()->getMapObjectsByGridRect(mainCharacter->getGridRect(), Trigger::RIDE) };
    
    // 何も見つからなかった場合は、UNDIFINEDをセットする
    if(objs.empty()) this->riddenEventID = etoi(EventID::UNDIFINED);
    
    for(MapObject* obj : objs)
    {
        if(obj->getEventId() != this->riddenEventID)
        {
            if(this->riddenEventID == etoi(EventID::UNDIFINED)) this->riddenEventID = obj->getEventId();
            DungeonSceneManager::getInstance()->pushEventBack(obj->getEventId());
        }
    }
}

// 一マス分移動し終えた時
void PlayerControlTask::onPartyMovedOneGrid(Party* party)
{
    // キューにあるイベントを実行
    DungeonSceneManager::getInstance()->runEventQueue();
    
    if(this->enableControl) this->walk(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
}

// 操作可能状態か設定
void PlayerControlTask::setControlEnable(bool enable, Party* party)
{
    bool before {this->enableControl};
    
    this->enableControl = enable;
    
    // 有効にされた時は、入力しているキーに応じて移動開始
    if(!before && enable) this->walk(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
}

// 操作可能状態か確認
bool PlayerControlTask::isControlEnabled()
{
    if(!this->enableControl) return false;
    
    // 疲労状態でない、またはスタミナが最大であれば操作可能
    return !DungeonSceneManager::getInstance()->getStamina()->isExhausted();
}

// スタミナが上限まで回復した時
void PlayerControlTask::onStaminaIncreasedMax()
{
    SoundManager::getInstance()->stopBGM(Resource::BGM::TIRED);
}