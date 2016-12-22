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
    this->unschedule(CC_SCHEDULE_SELECTOR(PlayerControlTask::update));
    CC_SAFE_RELEASE_NULL(_state);
}

// 初期化
bool PlayerControlTask::init()
{
    if (!GameTask::init()) return false;

    this->setCurrentState(SearchState::create(this));
    
    DungeonSceneManager::getInstance()->getStamina()->onIncreasedMax = CC_CALLBACK_0(PlayerControlTask::onStaminaIncreasedMax, this);
    
    this->schedule(CC_SCHEDULE_SELECTOR(PlayerControlTask::update), 0.1f);
    
    return true;
}

// Stateを設定
void PlayerControlTask::setCurrentState(PlayerControlState* state)
{
    CC_SAFE_RETAIN(state);
    CC_SAFE_RELEASE_NULL(_state);
    _state = state;
}

// 向きを変える
void PlayerControlTask::turn(const Key& key, Party* party)
{
    if (!this->isControlEnabled()) return;
    if (party->getMainCharacter()->isInAttackMotion()) return;
    
    Direction direction { Direction::convertKey(key) };
    
    _state->turn(party, direction, DungeonSceneManager::getInstance()->isPressed(Key::DASH));
}

// 決定キーが押された時
void PlayerControlTask::onEnterKeyPressed(Party* party)
{
    if (!this->isControlEnabled()) return;
    _state->onEnterKeyPressed(party);
}

// 歩行中、あたり判定を行い次に向かう位置を決定する
void PlayerControlTask::move(const vector<Key>& keys, Party* party)
{
    if (!this->isControlEnabled()) return;
    if (keys.empty()) return;
    if (party->getMainCharacter()->isMoving()) return;
    if (party->getMainCharacter()->isInAttackMotion()) return;
    
    vector<Direction> directions { Direction::convertKeys(keys) };
    
    _state->move(party, directions, DungeonSceneManager::getInstance()->isPressed(Key::DASH));
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
    
    if (_enableControl) this->move(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
}

// 操作可能状態か設定
void PlayerControlTask::setControlEnable(bool enable, Party* party)
{
    bool before { _enableControl };
    _enableControl = enable;
    
    // 有効にされた時は、入力しているキーに応じて移動開始
    if (!before && enable) this->move(DungeonSceneManager::getInstance()->getPressedCursorKeys(), party);
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
    this->setCurrentState(BattleState::create(this));
}

// バトル終了時
void PlayerControlTask::onBattleFinished()
{
    this->setCurrentState(SearchState::create(this));
}

// update
void PlayerControlTask::update(float delta)
{
    // DASHキーが押されていない場合はスタミナ減少を止める
    if (!DungeonSceneManager::getInstance()->isPressed(Key::DASH)) {
        DungeonSceneManager::getInstance()->getStamina()->setDecreasing(false);
    }
}
