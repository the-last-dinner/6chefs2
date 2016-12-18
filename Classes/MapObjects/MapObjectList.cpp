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
    
    _availableObjects.clear();
    _disableObjects.clear();
    _enemies.clear();
    _pathObjects.clear();
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
    AttackDetector* attackDetector { AttackDetector::create(collisionDetector) };
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
    if (!_availableObjects.empty()) return;
    
    _availableObjects = objects;
}

// 無効オブジェクトを設定
void MapObjectList::setDisableObjects(const Vector<MapObject*>& objects)
{
    if (!_disableObjects.empty()) return;
    
    _disableObjects = objects;
}

// 地形オブジェクトを設定
void MapObjectList::setTerrainObjects(const Vector<TerrainObject*>& objects)
{
    if (!_terrainObjects.empty()) return;
    
    _terrainObjects = objects;
}

// 経路オブジェクト
void MapObjectList::setPathObjects(const Vector<PathObject*>& objects)
{
    if (!_pathObjects.empty()) return;
    
    _pathObjects = objects;
}

#pragma mark -
#pragma mark Collection Getter

// 有効なオブジェクトを全て取得
Vector<MapObject*> MapObjectList::getAllAvailableObjects() const
{
    return _availableObjects;
}

// 指定IDのマップオブジェクトを取得
MapObject* MapObjectList::getMapObject(int objId) const
{
    for (Enemy* enemy : _enemies) {
        if (enemy->getObjectId() == objId) return enemy;
    }
    
    for (MapObject* obj : _availableObjects) {
        if (obj->getObjectId() == objId) return obj;
    }
    
    return nullptr;
}

// 無効リストから指定IDのマップオブジェクトを取得
MapObject* MapObjectList::getMapObjectFromDisableList(int objId) const
{
    for (MapObject* obj : _disableObjects) {
        if (obj->getObjectId() == objId) return obj;
    }
    
    return nullptr;
}

Vector<MapObject*> MapObjectList::getMapObjects(const MapObject* obj, const Trigger trigger) const
{
    Vector<MapObject*> mapObjects {};
    
    for (auto other : _availableObjects) {
        if (MapUtils::intersectsGridRect(obj->getGridCollisionRect(), other->getGridRect())) {
            if (other->getTrigger() != trigger) continue;
            
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
    
    for (auto other : _availableObjects) {
        bool shouldAdd { false };
        
        if (MapUtils::intersectsGridRect(obj->getGridCollisionRect(directions), other->getGridRect())) {
            if(!needTriggerDetection) shouldAdd = true;
            if(other->getTrigger() == trigger) shouldAdd = true;
        }
        
        if (shouldAdd) mapObjects.pushBack(other);
    }
    
    return mapObjects;
}

// 指定トリガーのマップオブジェクトのEventIDベクタを取得
vector<int> MapObjectList::getEventIds(const Trigger trigger) const
{
    vector<int> eventIds {};
    
    for (auto obj : _availableObjects) {
        if (obj->getTrigger() == trigger) eventIds.push_back(obj->getEventId());
    }
    
    return eventIds;
}

#pragma mark -
#pragma mark Available MapObject

// マップオブジェクトを追加
void MapObjectList::add(MapObject* mapObject)
{
    // もうすでにリスト内にあればリターン
    if (_availableObjects.find(mapObject) != _availableObjects.end()) return;
    
    _availableObjects.pushBack(mapObject);
    
    // 無効リストに存在する場合は削除
    for (MapObject* obj : _disableObjects) {
        if (mapObject->getObjectId() == obj->getObjectId()) {
            _disableObjects.eraseObject(mapObject);
            break;
        }
    }
}

// マップオブジェクトを削除
void MapObjectList::removeById(const int objectId)
{
    for (Enemy* enemy : _enemies) {
        if (enemy->getObjectId() != objectId) continue;
        this->removeEnemy(enemy);
    }
    
    for (MapObject* obj : _availableObjects) {
        if (obj->getObjectId() != objectId) continue;
        
        _disableObjects.pushBack(obj);
        _availableObjects.eraseObject(obj);
        obj->onExitMap();
        obj->removeFromParent();
    }
}

#pragma mark -
#pragma mark Enemy

// 敵を追加
void MapObjectList::addEnemy(Enemy* enemy)
{
    if(!enemy) return;
    
    _enemies.pushBack(enemy);
}

// EnemyIdから敵を削除
void MapObjectList::removeEnemyById(const int enemyId)
{
    for (Enemy* enemy : _enemies) {
        if (enemy->getEnemyId() == enemyId) {
            this->removeEnemy(enemy);
        }
    }
}

// ObjectIdから敵を削除
void MapObjectList::removeEnemyByObjectId(const int objectId)
{
    for (Enemy* enemy : _enemies) {
        if (!enemy) continue;
        if (enemy->getObjectId() == objectId) {
            this->removeEnemy(enemy);
        }
    }
}

// 対象の敵のオブジェクトを削除
void MapObjectList::removeEnemy(Enemy *enemy)
{
    _disableObjects.pushBack(enemy);
    enemy->removeFromParent();
    enemy->onExitMap();
    _enemies.eraseObject(enemy);
}

// 敵を全て取得
Vector<Enemy*> MapObjectList::getEnemiesAll()
{
    return _enemies;
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
    party->getMainCharacter()->setLostHPCallback([this](MapObject* obj) {
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
    for(auto enemy : _enemies)
    {
        enemy->onPartyMoved();
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
    for (PathObject* obj : _pathObjects) {
        if (obj->getPathId() != pathId) continue;
        
        return obj;
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
    for (auto obj : _availableObjects) {
        obj->onEventStart();
    }
    
    for (auto enemy : _enemies) {
        enemy->onEventStart();
    }
}

// 全オブジェクトにイベント終了を通知
void MapObjectList::onEventFinished()
{
    for (auto obj : _availableObjects) {
        obj->onEventFinished();
    }
    
    for (auto enemy : _enemies) {
        enemy->onEventFinished();
    }
}

#pragma mark -
#pragma mark Battle

// バトル開始時
void MapObjectList::onBattleStart(Battle* battle)
{
    for (auto obj : _availableObjects) {
        obj->onBattleStart(battle);
    }
    
    for (auto enemy : _enemies) {
        enemy->onBattleStart(battle);
    }
}

// バトル終了時
void MapObjectList::onBattleFinished()
{
    for (auto obj : _availableObjects) {
        obj->onBattleFinished();
    }
    
    for (auto enemy : _enemies) {
        enemy->onBattleFinished();
    }
}

#pragma mark -
#pragma mark Update

void MapObjectList::update(float delta)
{
    if (!_attackDetector) return;
    
    _attackDetector->update(delta);
}
