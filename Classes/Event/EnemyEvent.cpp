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
    if(!GameEvent::init()) return false;
    
    // 空のデータ生成
    EnemyData data {};
    
    // 敵ID
    if(this->eventHelper->hasMember(json, member::ENEMY_ID))
    {
        data.enemy_id = stoi(json[member::ENEMY_ID].GetString());
    }
    
    // キャラクタID
    if(!this->eventHelper->hasMember(json, member::CHARA_ID)) return false;
    data.chara_data.chara_id = stoi(json[member::CHARA_ID].GetString());
    
    // オブジェクトID
    if(!this->eventHelper->hasMember(json, member::OBJECT_ID)) return false;
    data.chara_data.obj_id = stoi(json[member::OBJECT_ID].GetString());
    
    // 向き
    Direction direction {this->eventHelper->getDirection(json)};
    if(direction == Direction::SIZE) direction = Direction::FRONT;
    data.chara_data.location.direction = direction;
    
    // マス座標
    data.chara_data.location.x = json[member::X].GetInt();
    data.chara_data.location.y = json[member::Y].GetInt();
    
    // 現在のマップID
    data.chara_data.location.map_id = DungeonSceneManager::getInstance()->getLocation().map_id;
    
    // 追跡アルゴリズム
    data.move_pattern = this->eventHelper->getMovePatternForEnemy(json);
    
    // 消えるまでに必要なマップ移動の回数
    if(this->eventHelper->hasMember(json, member::TIMES)) data.change_map_counter = json[member::TIMES].GetInt();
    
    // 移動速度の倍率
    if(this->eventHelper->hasMember(json, member::SPEED)) data.speed_ratio = json[member::SPEED].GetDouble();
    
    // 最初に目指す移動経路オブジェクトのID
    if(this->eventHelper->hasMember(json, member::PATH_ID)) data.start_path_id = stoi(json[member::PATH_ID].GetString());
    
    // データを格納
    this->data = data;
    
    return true;
}

void CreateEnemyEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->addEnemy(Enemy::create(this->data));
}

#pragma mark -
#pragma mark RemoveEnemyEvent

bool RemoveEnemyEvent::init(rapidjson::Value& json)
{
    if(!GameEvent::init()) return false;
    
    bool hasEnemyId = this->eventHelper->hasMember(json, member::ENEMY_ID);
    bool hasObjectId = this->eventHelper->hasMember(json, member::OBJECT_ID);
    
    if (!hasEnemyId && !hasObjectId) return false;
    
    if (hasEnemyId) {
        this->enemyId = stoi(json[member::ENEMY_ID].GetString());
    }
    if (hasObjectId) {
        this->objectId = stoi(json[member::OBJECT_ID].GetString());
    }
    
    return true;
}

void RemoveEnemyEvent::run()
{
    this->setDone();
    if (this->enemyId != etoi(EnemyID::UNDIFINED)) {
        DungeonSceneManager::getInstance()->removeEnemy(this->enemyId);
    }
    if (this->objectId != etoi(ObjectID::UNDIFINED)) {
        cout << "removeEnemy" << this->objectId << endl;
        DungeonSceneManager::getInstance()->removeEnemyByObjectId(this->objectId);
    }
}
