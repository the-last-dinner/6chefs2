//
//  EnemyData.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#ifndef LastSupper_EnemyData_h
#define LastSupper_EnemyData_h

#include "Datas/MapObject/CharacterData.h"
#include "Models/ChangeMapHistory.h"

struct EnemyData
{
    int enemy_id { static_cast<int>(EnemyID::UNDIFINED) };
    int change_map_counter { -1 };
    float speed_ratio { 1.0f };
    int start_path_id { -1 };
    EnemyMovePattern move_pattern {EnemyMovePattern::SIZE};
    CharacterData chara_data {};
};

struct SummonData
{
    deque<float> summon_delays {};
    ChangeMapHistory history {};
    EnemyData enemy_data {};
    bool canMove { true };
    bool isDone { false };
    bool isDeleted { false };
    
    void addHistory(const Relation& relation, const float summon_delay) { history.add(relation); summon_delays.push_back(summon_delay); };
    void deleteLatestHistory() { history.deleteLatest(); summon_delays.pop_back(); };
    void deleteOldestHistory() { history.deleteOldest(); summon_delays.pop_front(); };
    bool existsHistory() const { return !summon_delays.empty(); };
};

#endif
