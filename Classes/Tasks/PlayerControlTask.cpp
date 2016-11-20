//
//  PlayerControlTask.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/30.
//
//

#include "Tasks/PlayerControlTask.h"

#include "Tasks/PlayerControlState/PlayerControlState.h"
#include "Tasks/PlayerControlState/SearchState.h"
#include "Tasks/PlayerControlState/BattleState.h"

#include "MapObjects/Character.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/Party.h"
#include "MapObjects/Status/Stamina.h"
#include "MapObjects/TerrainObject/TerrainObject.h"

#include "Managers/DungeonSceneManager.h"

// 定数
const string PlayerControlTask::START_WALKING_SCHEDULE_KEY { "start_walking" };
const float PlayerControlTask::DASH_SPEED_RATIO {2.0f};

// コンストラクタ
PlayerControlTask::PlayerControlTask() { FUNCLOG }

// デストラクタ
PlayerControlTask::~PlayerControlTask()
{
    FUNCLOG

    CC_SAFE_RELEASE_NULL(_state);
}

// 初期化
bool PlayerControlTask::init()
{
    if (!GameTask::init()) return false;

    this->setCurrentState(SearchState::create());
    
    DungeonSceneManager::getInstance()->getStamina()->onIncreasedMax = CC_CALLBACK_0(PlayerControlTask::onStaminaIncreasedMax, this);
    
    return true;
}

// Stateを設定
void PlayerControlTask::setCurrentState(PlayerControlState* state)
{
    CC_SAFE_RELEASE_NULL(_state);
    CC_SAFE_RETAIN(state);
    _state = state;
}

// 向きを変える
void PlayerControlTask::turn(const Key& key, Party* party)
{
    if (!this->isControlEnabled()) return;
    
    Direction direction { Direction::convertKey(key) };
    Character* mainCharacter {party->getMainCharacter()};
    
    // 主人公の向きを変更
    mainCharacter->setDirection(direction);
    
    // 主人公が移動中でなければ
    if (!mainCharacter->isMoving()) {
        // 一定時間後に歩行開始
        if (!this->isScheduled(START_WALKING_SCHEDULE_KEY)) this->scheduleOnce([this, party](float _) {
            this->walk(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
        }, 0.0f, START_WALKING_SCHEDULE_KEY);
    }
}

// 決定キーが押された時
void PlayerControlTask::onEnterKeyPressed(Party* party)
{
    if (!this->isControlEnabled()) return;
    
    _state->onEnterKeyPressed(party);
}

// 歩行中、あたり判定を行い次に向かう位置を決定する
void PlayerControlTask::walk(const vector<Key>& keys, Party* party)
{
    if (keys.empty() || !this->isControlEnabled() || party->getMainCharacter()->isMoving()) return;
    
    vector<Direction> directions { Direction::convertKeys(keys) };
    
    Character* mainCharacter { party->getMainCharacter() };
    
    // ダッシュキーが押されていたら、速度の倍率をあげる
    bool dash { mainCharacter->isRunnable() ? DungeonSceneManager::getInstance()->isPressed(Key::DASH) : false };
    
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
    
    party->move(moveDirections, dash ? DASH_SPEED_RATIO : 1.f, [this, party, stamina, dash](bool moved) {
        if (!moved) return;
        
        stamina->setDecreasing(false);
        this->onPartyMovedOneGrid(party, dash);
    });
}

// 一マス分移動し終えた時
void PlayerControlTask::onPartyMovedOneGrid(Party* party, bool dashed)
{
    Stamina* stamina { DungeonSceneManager::getInstance()->getStamina() };
    Character* mainCharacter { party->getMainCharacter() };
    
    // 地形から、スタミナ減少の倍率を取得しセット
    stamina->setStepRatio(mainCharacter->getStaminaConsumptionRatio());
    
    // 敵出現中かつ、ダッシュ中ならスタミナを減少モードにする
    stamina->setDecreasing(DungeonSceneManager::getInstance()->existsEnemy() && (dashed || mainCharacter->consumeStaminaWalking()));
    
    // スタミナが危険値まで下がっていたら、bgmを再生
    if (stamina->isWarn() && !SoundManager::getInstance()->isPlaying(Resource::BGM::TIRED)) SoundManager::getInstance()->playBGM(Resource::BGM::TIRED, true, 2.0f);
    
    Vector<MapObject*> objs { DungeonSceneManager::getInstance()->getMapObjectList()->getMapObjects(mainCharacter, Trigger::RIDE) };
    
    // 何も見つからなかった場合は、UNDIFINEDをセットする
    if (objs.empty()) _riddenEventID = etoi(EventID::UNDIFINED);
    
    for(MapObject* obj : objs) {
        if (obj->getEventId() != _riddenEventID) {
            if (_riddenEventID == etoi(EventID::UNDIFINED)) _riddenEventID = obj->getEventId();
            DungeonSceneManager::getInstance()->pushEventBack(obj->getEventId());
        }
    }
    
    // キューにあるイベントを実行
    DungeonSceneManager::getInstance()->runEventQueue();
    
    if (_enableControl) this->walk(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
}

// 操作可能状態か設定
void PlayerControlTask::setControlEnable(bool enable, Party* party)
{
    bool before { _enableControl };
    
    _enableControl = enable;
    
    // 有効にされた時は、入力しているキーに応じて移動開始
    if (!before && enable) this->walk(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
}

// 操作可能状態か確認
bool PlayerControlTask::isControlEnabled()
{
    if (!_enableControl) return false;
    
    // 疲労状態でない、またはスタミナが最大であれば操作可能
    return !DungeonSceneManager::getInstance()->getStamina()->isExhausted();
}

// スタミナが上限まで回復した時
void PlayerControlTask::onStaminaIncreasedMax()
{
    SoundManager::getInstance()->stopBGM(Resource::BGM::TIRED);
}

// バトル開始時
void PlayerControlTask::onBattleStart()
{
    this->setCurrentState(BattleState::create());
}

// バトル終了時
void PlayerControlTask::onBattleFinished()
{
    this->setCurrentState(SearchState::create());
}
