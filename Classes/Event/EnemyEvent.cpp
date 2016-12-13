//
//  EnemyEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#include "Event/EnemyEvent.h"

#include "Event/EventScriptMember.h"
#include "Event/GameEventHelper.h"

#include "MapObjects/Enemy.h"
#include "MapObjects/MapObjectList.h"

#include "Managers/DungeonSceneManager.h"

#pragma mark CreateEnemyEvent

bool CreateEnemyEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init(json)) return false;
    
    // 空のデータ生成
    EnemyData data {};
    
    // 敵ID
    if (_eventHelper->hasMember(_json, member::ENEMY_ID)) {
        data.enemy_id = stoi(_json[member::ENEMY_ID].GetString());
    }
    
    // キャラクタID
    if (!_eventHelper->hasMember(_json, member::CHARA_ID)) return false;
    data.chara_data.chara_id = stoi(_json[member::CHARA_ID].GetString());
    
    // オブジェクトID
    if (!_eventHelper->hasMember(_json, member::OBJECT_ID)) return false;
    data.chara_data.obj_id = stoi(_json[member::OBJECT_ID].GetString());
    
    // 向き
    Direction direction {_eventHelper->getDirection(_json)};
    if (direction.isNull()) direction = Direction::DOWN;
    data.chara_data.location.direction = direction;
    
    // マス座標
    data.chara_data.location.x = _json[member::X].GetInt();
    data.chara_data.location.y = _json[member::Y].GetInt();
    
    // 現在のマップID
    data.chara_data.location.map_id = DungeonSceneManager::getInstance()->getLocation().map_id;
    
    // 追跡アルゴリズム
    data.move_pattern = _eventHelper->getMovePatternForEnemy(_json);
    
    // 消えるまでに必要なマップ移動の回数
    if (_eventHelper->hasMember(_json, member::TIMES)) data.change_map_counter = _json[member::TIMES].GetInt();
    
    // 移動速度の倍率
    if (_eventHelper->hasMember(_json, member::SPEED)) data.speed_ratio = _json[member::SPEED].GetDouble();
    
    // 最初に目指す移動経路オブジェクトのID
    if (_eventHelper->hasMember(_json, member::PATH_ID)) data.start_path_id = stoi(_json[member::PATH_ID].GetString());
    
    // データを格納
    _data = data;
    
    return true;
}

void CreateEnemyEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->addEnemy(Enemy::create(_data));
}

#pragma mark -
#pragma mark RemoveEnemyEvent

bool RemoveEnemyEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init(json)) return false;
    
    bool hasEnemyId = _eventHelper->hasMember(_json, member::ENEMY_ID);
    bool hasObjectId = _eventHelper->hasMember(_json, member::OBJECT_ID);
    
    if (!hasEnemyId && !hasObjectId) return false;
    
    if (hasEnemyId) {
        _enemyId = stoi(_json[member::ENEMY_ID].GetString());
    }
    if (hasObjectId) {
        _objectId = stoi(_json[member::OBJECT_ID].GetString());
    }
    
    return true;
}

void RemoveEnemyEvent::run()
{
    this->setDone();
    if (_enemyId != etoi(EnemyID::UNDIFINED)) {
        DungeonSceneManager::getInstance()->removeEnemy(_enemyId);
    }
    if (_objectId != etoi(ObjectID::UNDIFINED)) {
        DungeonSceneManager::getInstance()->removeEnemyByObjectId(_objectId);
    }
}
