//
//  MapObjectList.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/02.
//
//

#include "MapObjects/MapObjectList.h"

#include "MapObjects/Party.h"
#include "MapObjects/TerrainObject/PlainArea.h"
#include "MapObjects/DetectionBox/AttackDetector.h"
#include "MapObjects/DetectionBox/CollisionDetector.h"
#include "MapObjects/PathFinder/PathFinder.h"

// コンストラクタ
MapObjectList::MapObjectList() { FUNCLOG };

// デストラクタ
MapObjectList::~MapObjectList()
{
    FUNCLOG

    for(pair<int, MapObject*> objWithId : _availableObjects)
    {
        CC_SAFE_RELEASE(objWithId.second);
    }
    
    for(pair<int, MapObject*> objWithId : _disableObjects)
    {
        CC_SAFE_RELEASE(objWithId.second);
    }
    
    for(pair<int, MapObject*> objWithId : _enemies)
    {
        CC_SAFE_RELEASE(objWithId.second);
    }

    _terrainObjects.clear();
    
    CC_SAFE_RELEASE_NULL(_plainArea);
    CC_SAFE_RELEASE_NULL(_collisionDetector);
    CC_SAFE_RELEASE_NULL(_attackDetector);
    CC_SAFE_RELEASE_NULL(_pathFinder);
};

#pragma mark -
#pragma mark Initializer

// 初期化
bool MapObjectList::init(const Size& mapSize)
{
    // ノーマルの地形を生成
    PlainArea* plainArea { PlainArea::create() };
    CC_SAFE_RETAIN(plainArea);
    _plainArea = plainArea;
    
    // 当たり判定を生成
    CollisionDetector* collisionDetector { CollisionDetector::create() };
    CC_SAFE_RETAIN(collisionDetector);
    _collisionDetector = collisionDetector;
    
    // 攻撃判定を生成
    AttackDetector* attackDetector { AttackDetector::create() };
    CC_SAFE_RETAIN(attackDetector);
    _attackDetector = attackDetector;
    
    // 経路探索
    PathFinder* pathFinder { PathFinder::create(this, mapSize) };
    CC_SAFE_RETAIN(pathFinder);
    _pathFinder = pathFinder;
    
    this->scheduleUpdate();
    
    return true;
}

// 有効オブジェクトリストを設定
void MapObjectList::setAvailableObjects(const Vector<MapObject*>& objects)
{
    if(!_availableObjects.empty()) return;
    
    for(MapObject* obj : objects)
    {
        if(!obj) continue;
        
        CC_SAFE_RETAIN(obj);
        _availableObjects.insert({obj->getObjectId(), obj});
    }
}

// 無効オブジェクトを設定
void MapObjectList::setDisableObjects(const Vector<MapObject*>& objects)
{
    if(!_disableObjects.empty()) return;
    
    for(MapObject* obj : objects)
    {
        if(!obj) continue;
        
        CC_SAFE_RETAIN(obj);
        _disableObjects.insert({obj->getObjectId(), obj});
    }
}

// 地形オブジェクトを設定
void MapObjectList::setTerrainObjects(const Vector<TerrainObject*>& objects)
{
    if(!_terrainObjects.empty()) return;
    
    _terrainObjects = objects;
}

// 経路オブジェクト
void MapObjectList::setPathObjects(const Vector<PathObject*>& objects)
{
    if(!_pathObjects.empty()) return;
    
    for(PathObject* obj : objects)
    {
        if(!obj) continue;
        
        CC_SAFE_RETAIN(obj);
        _pathObjects.insert({obj->getPathId(), obj});
    }
}

#pragma mark -
#pragma mark Collection Getter

// 有効なオブジェクトを全て取得
Vector<MapObject*> MapObjectList::getAllAvailableObjects() const
{
    Vector<MapObject*> objs {};
    
    for(auto objWithId : _availableObjects)
    {
        objs.pushBack(objWithId.second);
    }
    
    return objs;
}

// 指定IDのマップオブジェクトを取得
MapObject* MapObjectList::getMapObject(int objId) const
{
    auto eItr { _enemies.find(objId) };
    if(eItr != _enemies.end())
    {
        return eItr->second;
    }
    
    auto aItr { _availableObjects.find(objId) };
    if(aItr != _availableObjects.end())
    {
        return aItr->second;
    }
    
    return nullptr;
}

// 無効リストから指定IDのマップオブジェクトを取得
MapObject* MapObjectList::getMapObjectFromDisableList(int objId) const
{
    auto itr { _disableObjects.find(objId) };
    if(itr == _disableObjects.end()) return nullptr;
    
    return itr->second;
}

Vector<MapObject*> MapObjectList::getMapObjects(const MapObject* obj, const Trigger trigger) const
{
    Vector<MapObject*> mapObjects {};
    
    for(auto objWithId : _availableObjects)
    {
        MapObject* other { objWithId.second };
        
        if(MapUtils::intersectsGridRect(obj->getGridCollisionRect(), other->getGridRect()))
        {
            if(other->getTrigger() != trigger) continue;
            
            mapObjects.pushBack(other);
        }
    }
    
    return mapObjects;
}

// MapObject、方向、トリガーからマップオブジェクトを取得
Vector<MapObject*> MapObjectList::getMapObjects(const MapObject* obj, const vector<Direction>& directions, const Trigger trigger) const
{
    Vector<MapObject*> mapObjects {};
    bool needTriggerDetection { trigger != Trigger::SIZE };
    
    for(auto objWithId : _availableObjects)
    {
        MapObject* other { objWithId.second };
        
        bool shouldAdd { false };
        
        if(MapUtils::intersectsGridRect(obj->getGridCollisionRect(directions), other->getGridRect()))
        {
            if(!needTriggerDetection) shouldAdd = true;
            if(other->getTrigger() == trigger) shouldAdd = true;
        }
        
        if(shouldAdd) mapObjects.pushBack(other);
    }
    
    return mapObjects;
}

// 指定トリガーのマップオブジェクトのEventIDベクタを取得
vector<int> MapObjectList::getEventIds(const Trigger trigger) const
{
    vector<int> eventIds {};
    
    for(auto objWithId : _availableObjects)
    {
        MapObject* obj { objWithId.second };
        
        if(obj->getTrigger() == trigger) eventIds.push_back(obj->getEventId());
    }
    
    return eventIds;
}

#pragma mark -
#pragma mark Available MapObject

// マップオブジェクトを追加
void MapObjectList::add(MapObject* mapObject)
{
    if(_availableObjects.count(mapObject->getObjectId()) != 0) return;
        
    CC_SAFE_RETAIN(mapObject);
    _availableObjects.insert({mapObject->getObjectId(), mapObject});
    
    auto itr { _disableObjects.find(mapObject->getObjectId()) };
    if(itr == _disableObjects.end()) return;
    
    CC_SAFE_RELEASE(itr->second);
    _disableObjects.erase(itr);
}

// マップオブジェクトを削除
void MapObjectList::removeById(const int objectId)
{
    auto eItr { _enemies.find(objectId) };
    if(eItr != _enemies.end())
    {
        _disableObjects.insert({objectId, eItr->second});
        _enemies.erase(objectId);
        eItr->second->onExitMap();
        eItr->second->removeFromParent();
    }
    
    auto itr { _availableObjects.find(objectId) };
    if(itr != _availableObjects.end())
    {
        _disableObjects.insert({objectId, itr->second});
        _availableObjects.erase(objectId);
        itr->second->onExitMap();
        itr->second->removeFromParent();
    }
}

#pragma mark -
#pragma mark Enemy

// 敵を追加
void MapObjectList::addEnemy(Enemy* enemy)
{
    if(!enemy) return;
    
    CC_SAFE_RETAIN(enemy);
    _enemies.insert({enemy->getObjectId(), enemy});
}

// EnemyIdから敵を削除
void MapObjectList::removeEnemyById(const int enemyId)
{
    for (Enemy* enemy : this->enemies) {
        if(enemy->getEnemyId() == enemyId) {
            this->removeEnemy(enemy);
        }
    }
}

// ObjectIdから敵を削除
void MapObjectList::removeEnemyByObjectId(const int objectId)
{
    for (Enemy* enemy : this->enemies) {
        if(enemy->getObjectId() == objectId) {
            this->removeEnemy(enemy);
        }
    }
}

// 対象の敵のオブジェクトを削除
void MapObjectList::removeEnemy(Enemy *enemy)
{
    std::mutex mtx;
    mtx.lock();
    enemy->removeFromParent();
    this->enemies.eraseObject(enemy);
    mtx.unlock();
}

// 敵を全て取得
Vector<Enemy*> MapObjectList::getEnemiesAll()
{
    Vector<Enemy*> enemies {};
    
    for(auto enemyWithId : _enemies)
    {
        enemies.pushBack(enemyWithId.second);
    }
    
    return enemies;
}

// 敵が存在するか
bool MapObjectList::existsEnemy() const
{
    return !_enemies.empty();
}

#pragma mark -
#pragma mark Party

// 主人公一行を格納
void MapObjectList::setParty(Party* party)
{
    // 一行が動いた時のコールバックを設定
    party->onPartyMoved = CC_CALLBACK_1(MapObjectList::onPartyMoved, this);
    
    // 主人公のHPがなくなった時のコールバックを設定
    party->getMainCharacter()->setLostHPCallback([this](Character* chara) {
        this->unscheduleUpdate();
        if (!_onLostMainCharacterHP) return;
        _onLostMainCharacterHP();
    });
    
    _party = party;
}

// 主人公一行を取得
Party* MapObjectList::getParty()
{
    return _party;
}

// 主人公一行が移動した時
void MapObjectList::onPartyMoved(const Rect& gridRect)
{
    // 敵に移動したことを通知する
    for(auto enemyWithId : _enemies)
    {
        enemyWithId.second->onPartyMoved();
    }
}

#pragma mark -
#pragma mark TerrainObject

TerrainObject* MapObjectList::getTerrain(MapObject* mapObject, const vector<Direction>& directions)
{
    Rect gridRect { mapObject->getGridRect(directions) };
    
    for(TerrainObject* obj : _terrainObjects)
    {
        if(!MapUtils::includesGridRect(obj->getGridRect(), gridRect)) continue;
        
        return obj;
    }
    
    // 何も見つからなかった場合はノーマル地形を返す
    return _plainArea;
}

#pragma mark -
#pragma mark PathObject

// 経路オブジェクトをIDから取得
PathObject* MapObjectList::getPathObjectById(const int pathId)
{
    auto itr { _pathObjects.find(pathId) };
    if(itr != _pathObjects.end())
    {
        return itr->second;
    }
    
    return nullptr;
}

#pragma mark -
#pragma mark CollisionDetector

CollisionDetector* MapObjectList::getCollisionDetector() const
{
    return _collisionDetector;
}

#pragma mark -
#pragma mark AttackDetector

AttackDetector* MapObjectList::getAttackDetector() const
{
    return _attackDetector;
}

#pragma mark -
#pragma mark PathFinder

PathFinder* MapObjectList::getPathFinder() const
{
    return _pathFinder;
}

#pragma mark -
#pragma mark Event

// 全オブジェクトにイベント開始を通知
void MapObjectList::onEventStart()
{
    for(auto objWithId : _availableObjects)
    {
        objWithId.second->onEventStart();
    }
    
    for(auto enemyWithId : _enemies)
    {
        enemyWithId.second->onEventStart();
    }
}

// 全オブジェクトにイベント終了を通知
void MapObjectList::onEventFinished()
{
    for(auto objWithId : _availableObjects)
    {
        objWithId.second->onEventFinished();
    }
    
    for(auto enemyWithId : _enemies)
    {
        enemyWithId.second->onEventFinished();
    }
}

#pragma mark -
#pragma mark Update

void MapObjectList::update(float delta)
{
    if (!_attackDetector) return;
    
    _attackDetector->update(delta);
}
