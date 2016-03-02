//
//  EnemyEvent.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#include "Event/EnemyEvent.h"

#include "Event/EventScriptMember.h"
#include "Event/EventScriptValidator.h"

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
    if(!this->validator->hasMember(json, member::ENEMY_ID)) return false;
    data.enemy_id = stoi(json[member::ENEMY_ID].GetString());
    
    // キャラクタID
    if(!this->validator->hasMember(json, member::CHARA_ID)) return false;
    data.chara_data.chara_id = stoi(json[member::CHARA_ID].GetString());
    
    // 向き
    Direction direction {this->validator->getDirection(json)};
    if(direction == Direction::SIZE) direction = Direction::FRONT;
    data.chara_data.location.direction = direction;
    
    // マス座標
    data.chara_data.location.x = json[member::X].GetInt();
    data.chara_data.location.y = json[member::Y].GetInt();
    
    // 現在のマップID
    data.chara_data.location.map_id = DungeonSceneManager::getInstance()->getLocation().map_id;
    
    // 追跡アルゴリズム
    data.move_pattern = this->validator->getMovePatternForEnemy(json);
    
    // 消えるまでに必要なマップ移動の回数
    if(this->validator->hasMember(json, member::TIMES)) data.change_map_counter = json[member::TIMES].GetInt();
    
    // 移動速度の倍率
    if(this->validator->hasMember(json, member::SPEED)) data.speed_ratio = json[member::SPEED].GetDouble();
    
    // 最初に目指す移動経路オブジェクトのID
    if(this->validator->hasMember(json, member::PATH_ID)) data.start_path_id = stoi(json[member::PATH_ID].GetString());
    
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
    
    // 敵ID
    if(!this->validator->hasMember(json, member::ENEMY_ID)) return false;
    this->enemyId = stoi(json[member::ENEMY_ID].GetString());
    
    return true;
}

void RemoveEnemyEvent::run()
{
    this->setDone();
    DungeonSceneManager::getInstance()->removeEnemy(this->enemyId);
}
