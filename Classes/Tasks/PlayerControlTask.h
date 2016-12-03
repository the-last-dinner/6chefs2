//
//  PlayerControlTask.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/30.
//
//

#ifndef __LastSupper__ControlMainCharacterTask__
#define __LastSupper__ControlMainCharacterTask__

#include "Tasks/GameTask.h"

class PlayerControlState;
class EventListenerKeyboardLayer;
class Party;

class PlayerControlTask : public GameTask
{
// 定数
private:
    static const string START_WALKING_SCHEDULE_KEY;
public:
    static const float DASH_SPEED_RATIO;

// クラスメソッド
public:
    CREATE_FUNC(PlayerControlTask)

// インスタンス変数
private:
    PlayerControlState* _state { nullptr };
    int _riddenEventID { static_cast<int>(EventID::UNDIFINED) };
    bool _enableControl { false };
    
// インスタンスメソッド
private:
    PlayerControlTask();
    ~PlayerControlTask();
    bool init();
    void setCurrentState(PlayerControlState* state);
public:
    void turn(const Key& key, Party* party);
    void onEnterKeyPressed(Party* party);
    void move(const vector<Key>& keys, Party* party);
    void onPartyMovedOneGrid(Party* party, bool dashed);
    void setControlEnable(bool enable, Party* party);
    bool isControlEnabled();
    void onStaminaIncreasedMax();
    void onBattleStart();
    void onBattleFinished();
};

#endif /* defined(__LastSupper__ControlMainCharacterTask__) */
