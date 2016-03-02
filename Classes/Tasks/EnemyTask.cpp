//
//  EnemyTask.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/21.
//
//

#include "Tasks/EnemyTask.h"

#include "MapObjects/Enemy.h"
#include "MapObjects/MapObjectList.h"

#include "Managers/DungeonSceneManager.h"

// コンストラクタ
EnemyTask::EnemyTask() {FUNCLOG};

// デストラクタ
EnemyTask::~EnemyTask() {FUNCLOG};

// 初期化
bool EnemyTask::init()
{
    if(!GameTask::init()) return false;
    
    // 配置すべき敵の情報を格納
    this->datas = DungeonSceneManager::getInstance()->getSummonDatas();
    
    return true;
}

// 敵を召喚する必要があるか
bool EnemyTask::needsSummonEnemy(const SummonData& data) const
{
    return data.enemy_data.chara_data.location.map_id == this->currentMapId && !data.existsHistory();
}

// 敵を召喚
void EnemyTask::summonEnemy(SummonData& data)
{
    data.isDone = true;
    
    DungeonSceneManager::getInstance()->addEnemy(Enemy::create(data.enemy_data));
}

// データとして存在している敵の出現遅延時間を取得
float EnemyTask::calcSummonDelayForData(const SummonData& data, const Location& enterLocation, const Location& exitLocation) const
{
    Point enterPos { Point(enterLocation.x, enterLocation.y) };
    Point exitPos { Point(exitLocation.x, exitLocation.y) };
    
    float distance { fabs(exitPos.x - enterPos.x) + fabs(exitPos.y - enterPos.y) };
    
    return Enemy::DURATION_MOVE_ONE_GRID * distance / data.enemy_data.speed_ratio;
}

// 出現を開始
void EnemyTask::start(const int mapId)
{
    if(this->datas.empty()) return;
    
    this->currentMapId = mapId;
    
    this->update(0);
    
    this->schedule(CC_SCHEDULE_SELECTOR(EnemyTask::update), 0.5f);
}

// 敵の出現を止める
void EnemyTask::stop()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(EnemyTask::update));
}

// 敵を削除
void EnemyTask::removeEnemy(const int enemyId)
{
    // マップ上にいる敵を削除
    DungeonSceneManager::getInstance()->getMapObjectList()->removeEnemyById(enemyId);
    
    // データとして存在している敵を論理削除
    for(SummonData& data : this->datas)
    {
        if(data.enemy_data.enemy_id != enemyId) continue;
        data.isDeleted = true;
    }
    
    // 削除後に敵がいなくなればコールバックを呼ぶ
    if(!this->existsEnemy() && this->onAllEnemyRemoved) this->onAllEnemyRemoved();
}

// update
void EnemyTask::update(float delta)
{
    if(this->datas.empty()) return;
    
    for(SummonData& data : this->datas)
    {
        // 論理削除済みなら無視
        if(data.isDeleted) continue;
        
        // すでに出現済みの敵情報なら無視
        if(data.isDone) continue;
        
        // 動けない敵の居場所が現在地でない場合は無視
        if(!data.canMove && data.history.getOldestRelation().from_location.map_id != this->currentMapId) continue;
        
        // 経過時間を遅延時間から引く
        data.summon_delays.front() -= delta;
        
        // 遅延時間が0以下になっていない場合は無視
        if(data.summon_delays.front() > 0.0f) continue;
        
        // 最古の履歴の、来た場所のマップIDが現在のマップIDならば来た場所を、そうでなければ行き先を敵の居場所に設定
        if(data.history.getOldestRelation().from_location.map_id == this->currentMapId)
        {
            data.enemy_data.chara_data.location = data.history.getOldestRelation().from_location;
        }
        else
        {
            data.enemy_data.chara_data.location = data.history.getOldestRelation().to_location;
        }
        
        // 最古の履歴を削除
        data.deleteOldestHistory();
        
        // 敵に格納されているマップIDと、現在のマップIDが一緒なら敵を出現させる
        if(!this->needsSummonEnemy(data)) continue;
        
        this->summonEnemy(data);
    }
}

// 現在配置されている敵と、配置予定の敵から、次マップへの敵情報を生成する
vector<SummonData> EnemyTask::createDatas(const Vector<Enemy*>& enemies, const Location& destLocation, const Location& exitLocation, const Location& enterLocation) const
{
    vector<SummonData> datas {};
    
    // 現在配置されている敵について
    if(!enemies.empty())
    {
        for(Enemy* enemy : enemies)
        {
            // 出現情報生成
            SummonData data {};
            
            // 敵情報を格納
            data.enemy_data = enemy->getEnemyData();
            
            // マップ移動カウントを-1
            data.enemy_data.change_map_counter -= 1;
            
            // カウントが0なら引き継がない
            if(data.enemy_data.change_map_counter == 0) continue;
            
            // 現在位置、行き先を格納
            Relation relation {};
            relation.from_location = data.enemy_data.chara_data.location;
            relation.to_location = destLocation;
            
            float delay { 0.0f };
            
            // マップ間移動不可なら、移動不可という情報を付加する
            if(!enemy->canGoToNextMap())
            {
                data.canMove = false;
            }
            else
            {
                // 次マップに出現するまでの遅延時間を格納
                delay = enemy->calcSummonDelay() + 1.f;
            }
            
            data.addHistory(relation, delay);
            
            datas.push_back(data);
        }
    }
    
    // まだ配置されていない敵について
    if(!this->datas.empty())
    {
        // 主人公が利用した出口の位置と、行き先を格納
        Relation relation {};
        relation.from_location = exitLocation;
        relation.to_location = destLocation;
        
        for(SummonData data : this->datas)
        {
            // 論理削除済みなら無視
            if(data.isDeleted) continue;
            
            // すでに出現させていたら無視
            if(data.isDone) continue;
            
            // マップ移動カウントを-1
            data.enemy_data.change_map_counter -= 1;
            
            // カウントがなら引き継がない
            if(data.enemy_data.change_map_counter == 0) continue;
            
            // 動けない敵の場合はそのままpush
            if(!data.canMove)
            {
                datas.push_back(data);
                
                continue;
            }
            
            // 行き先が、来た場所ではない時
            if(data.history.getLatestRelation().from_location.map_id != destLocation.map_id)
            {
                // 履歴を追加
                // 遅延時間を、入り口から出口までの距離から計算する
                data.addHistory(relation, this->calcSummonDelayForData(data, enterLocation, exitLocation));
                
                datas.push_back(data);
                
                continue;
            }
            
            // 行き先が、元々来た場所だった場合は最新の履歴を追加しないで、遅延時間を0秒にする
            data.summon_delays.pop_back();
            data.summon_delays.push_back(0.0f);
            
            datas.push_back(data);
        }
    }
    
    return datas;
}

// 敵が存在するか
bool EnemyTask::existsEnemy() const
{
    // 論理削除をチェック
    bool empty {true};
    for(SummonData data : this->datas)
    {
        if(data.isDeleted) continue;
        empty = false;
        break;
    }
    
    // マップ上に配置されている、もしくはデータとして存在していれば、true
    return DungeonSceneManager::getInstance()->getMapObjectList()->existsEnemy() || !empty;
}