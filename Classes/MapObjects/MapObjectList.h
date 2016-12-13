//
//  MapObjectList.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/02.
//
//

#ifndef __LastSupper__MapObjectList__
#define __LastSupper__MapObjectList__

#include "Common.h"

#include "MapObjects/MapObject.h"
#include "MapObjects/Enemy.h"
#include "MapObjects/TerrainObject/TerrainObject.h"
#include "MapObjects/PathObject.h"

class Party;
class PathFinder;
class PlainArea;
class CollisionDetector;
class AttackDetector;
class Battle;

class MapObjectList : public Node
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(MapObjectList, const Size&)
    
// インスタンス変数
private:
    Vector<MapObject*> _availableObjects {};
    Vector<MapObject*> _disableObjects {};
    Vector<Enemy*> _enemies {};
    Vector<PathObject*> _pathObjects {};
    
    Party* _party { nullptr };
    Vector<TerrainObject*> _terrainObjects {};
    PlainArea* _plainArea { nullptr };
    CollisionDetector* _collisionDetector { nullptr };
    AttackDetector* _attackDetector { nullptr };
    PathFinder* _pathFinder { nullptr };
public:
    function<void()> _onLostMainCharacterHP { nullptr };
    
// インスタンスメソッド
private:
    MapObjectList();
    virtual ~MapObjectList();
    bool init(const Size& mapSize);
public:
    // 初期設定
    void setAvailableObjects(const Vector<MapObject*>& objects);
    void setDisableObjects(const Vector<MapObject*>& objects);
    void setTerrainObjects(const Vector<TerrainObject*>& objects);
    void setPathObjects(const Vector<PathObject*>& objects);
    
    // 取得系
    Vector<MapObject*> getAllAvailableObjects() const;
    MapObject* getMapObject(int objId) const;
    MapObject* getMapObjectFromDisableList(int objId) const;
    Vector<MapObject*> getMapObjects(const MapObject* obj, const Trigger trigger) const;
    Vector<MapObject*> getMapObjects(const MapObject* obj, const vector<Direction>& directions, const Trigger trigger = Trigger::SIZE) const;
    
    vector<int> getEventIds(const Trigger trigger) const;
    
    // 追加、削除
    void add(MapObject* mapObject);
    void removeById(const int objectId);
    
    // 敵
    void addEnemy(Enemy* enemy);
    void removeEnemyById(const int enemyId);
    void removeEnemyByObjectId(const int objectId);
    void removeEnemy(Enemy* enemy);
    Vector<Enemy*> getEnemiesAll();
    bool existsEnemy() const;
    
    // 主人公一行
    void setParty(Party* party);
    Party* getParty();
    void onPartyMoved(const Rect& gridRect);
    
    // 地形
    TerrainObject* getTerrain(MapObject* mapObject, const vector<Direction>& directions);
    
    // 経路オブジェクト
    PathObject* getPathObjectById(const int pathId);
    
    // 当たり判定
    CollisionDetector* getCollisionDetector() const;
    
    // 攻撃判定
    AttackDetector* getAttackDetector() const;
    
    // 経路探索
    void setPathFinder(PathFinder* pathFinder);
    PathFinder* getPathFinder() const;
    
    // イベント
    void onEventStart();
    void onEventFinished();
    
    // 戦闘
    void onBattleStart(Battle* battle);
    void onBattleFinished();

    // 敵と主人公一行の衝突監視用updateメソッド
    void update(float delta);
};

#endif /* defined(__LastSupper__MapObjectList__) */
