//
//  EquipItemEvent.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/13.
//
//

#ifndef EquipItemEvent_hpp
#define EquipItemEvent_hpp

#include "define.h"

class EventScript;

class EquipItemEvent : public Ref
{
public:
    static const string EVENT_SCRIPT_NAME;
    // クラスメソッド
public:
    CREATE_FUNC(EquipItemEvent)
private:
    EquipItemEvent(){FUNCLOG};
    ~EquipItemEvent(){FUNCLOG};
    
    // インスタンス変数
private:
    EventScript* _equipItemEvent {nullptr};
    int _rightItemIdCache {0};
    int _leftItemIdCache {0};
public:
    
    // インスタンスメソッド
private:
    bool init();
    
public:
    void resetEquipmentCache();
    void updateEquipment();
    void onChangeEquipment(const int rightItemId, const int leftItemId);
    void onReleaseItem(const int targetCache, const int rightItemId, const int leftItemId);
    void onEquipItem(const int targetItemId);
    void runCallbackEvent(const string& eventName);
};

#endif /* EquipItemEvent_hpp */
