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
class PlainArea;

class MapObjectList : public Node
{
// クラスメソッド
public:
    CREATE_FUNC(MapObjectList)
    
// インスタンス変数
private:
    Vector<MapObject*> availableObjects {};
    Vector<MapObject*> disableObjects {};
    Party* party { nullptr };
    Vector<Enemy*> enemies {};
    Vector<TerrainObject*> terrainObjects {};
    Vector<PathObject*> pathObjects {};
    PlainArea* plainArea { nullptr };
public:
    function<void()> onContactWithEnemy { nullptr };
    
// インスタンスメソッド
private:
    MapObjectList();
    ~MapObjectList();
    bool init();
public:
    // 初期設定
    void setAvailableObjects(const Vector<MapObject*>& objects);
    void setDisableObjects(const Vector<MapObject*>& objects);
    void setTerrainObjects(const Vector<TerrainObject*>& objects);
    void setPathObjects(const Vector<PathObject*>& objects);
    
    // 取得系
    MapObject* getMapObject(int objId) const;
    MapObject* getMapObjectFromDisableList(int objId) const;
    const Vector<MapObject*>& getMapObjects() const;
    Vector<MapObject*> getCollisionObjects(const vector<MapObject*> exclusion = {}) const;
    Vector<MapObject*> getMapObjects(const Rect& rect) const;
    Vector<MapObject*> getMapObjects(const Point& position) const;
    Vector<MapObject*> getMapObjectsByGridRect(const Rect& gridRect, const Trigger trigger = Trigger::SIZE) const;
    
    vector<int> getEventIds(const Trigger trigger) const;
    vector<int> getEventIdsByGridRect(const Rect& gridRect, const Trigger trigger) const;
    vector<Rect> getGridCollisionRects(MapObject* exclusion = nullptr) const;
    vector<Rect> getGridCollisionRects(vector<MapObject*> exclusion) const;
    vector<Rect> getCollisionRects(vector<MapObject*> exclusion) const;
    
    // 追加、削除
    void add(MapObject* mapObject);
    void removeById(const int objectId);
    
    // 敵
    void addEnemy(Enemy* enemy);
    void removeEnemyById(const int enemyId);
    Vector<Enemy*> getEnemiesAll();
    bool existsEnemy() const;
    
    // 主人公一行
    void setParty(Party* party);
    Party* getParty();
    void onPartyMoved(const Rect& gridRect);
    
    // 地形
    TerrainObject* getTerrainByGridRect(const Rect& gridRect);
    
    // 経路オブジェクト
    PathObject* getPathObjectById(const int pathId);
    
    // オブジェクトの自動移動
    void moveStartAllObjects();
    void moveStopAllObjects();

    // 敵と主人公一行の衝突監視用updateメソッド
    void update(float delta);
};

#endif /* defined(__LastSupper__MapObjectList__) */
