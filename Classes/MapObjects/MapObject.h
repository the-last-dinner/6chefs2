//
//  MapObject.h
//  LastSupper
//
//  Created by Kohei on 2015/06/21.
//
//

#ifndef __MAP_OBJECT_H__
#define __MAP_OBJECT_H__

#include "Common.h"

class Light;
class AmbientLightLayer;
class MapObjectList;
class MovePattern;
class TerrainObject;
class TerrainState;
class TerrainStateCache;
class CollisionBox;

class MapObject : public Node
{
// 定数
public:
    static const float DURATION_MOVE_ONE_GRID;
    
// インスタンス変数
private:
    int _objectId { static_cast<int>(ObjectID::UNDIFINED) };
    int _eventId { static_cast<int>(EventID::UNDIFINED) };
	Trigger _trigger { Trigger::SIZE };
    bool _isMovable { false };
	Light* _light { nullptr };
    bool _isMoving { false };
    Sprite* _sprite { nullptr };
    bool _paused { false };
    int _getOffEventID { static_cast<int>(EventID::UNDIFINED)};
    int _rideOnEventID { static_cast<int>(EventID::UNDIFINED)};
    vector<Direction> _movingDirections {};
    CollisionBox* _collision { nullptr };
    Label* _debugLabel { nullptr };
protected:
    MapObjectList* _objectList { nullptr };
    deque<vector<Direction>> _directionsQueue {};
    Location _location {};
    TerrainState* _terrainState { nullptr };
    TerrainStateCache* _terrainStateCache { nullptr };
    
public:
    function<void(MapObject*)> onMoved { nullptr };
	
// インスタンスメソッド
public:
	MapObject();
	virtual ~MapObject();
    virtual bool init();
	void setGridPosition(const Point& gridPosition);
    virtual void setDirection(const Direction& direction);
    void setObjectId(int objectId);
	void setEventId(int eventId);
	void setTrigger(Trigger trigger);
    void setMovable(bool isMovable);
    void setMapObjectList(MapObjectList* objectList);
    void setSprite(Sprite* sprite);
    void setPaused(bool paused);
    void setCollision(CollisionBox* collision);
    
	void setLight(Light* light, AmbientLightLayer* ambientLightLayer, function<void()> callback = nullptr);
	void removeLight(function<void()> callback = nullptr);
	
    Location getLocation() const;
    Size  getGridSize() const;
	Point getGridPosition() const;
    Rect getGridRect(const vector<Direction>& directions = {}) const;
    Rect getGridCollisionRect(const vector<Direction>& directions = {}) const;
    int getObjectId() const;
	int getEventId() const;
	Trigger getTrigger() const;
    bool isMoving() const;
    Direction getDirection() const;
    Sprite* getSprite() const;
    bool isPaused() const;
    vector<Direction> getMovingDirections() const;
    CollisionBox* getCollision() const;
    
    // collision
    Rect getCollisionRect() const;
    bool isHit() const;
    bool isHit(const Direction& direction) const;
    virtual bool isHit(const vector<Direction>& directions) const;
    virtual bool isHit(const MapObject* other) const;
    bool isMovable() const;
    Vector<MapObject*> getHitObjects(const Direction& direction) const;
    Vector<MapObject*> getHitObjects(const vector<Direction>& directions) const;
    
    // move
    vector<Direction> createEnableDirections(const vector<Direction>& directions) const;
    bool canMove(const vector<Direction>& directions) const;
    void move(const vector<Direction>& enableDirections, function<void()> onMoved, const float ratio = 1.0f);
    bool moveBy(const Direction& direction, function<void()> onMoved, const float ratio = 1.0f);
    bool moveBy(const vector<Direction>& directions, function<void()> onMoved, const float ratio = 1.0f);
    void moveBy(const Direction& direction, const int gridNum, function<void(bool)> onMoved, const float ratio = 1.0f);
    void moveBy(const vector<Direction>& directions, const int gridNum, function<void(bool)> onMoved, const float ratio = 1.0f);
    void moveByQueue(deque<Direction> directionsQueue, function<void(bool)> callback, const float ratio = 1.0f);
    void moveByQueue(deque<vector<Direction>> directionsQueue, function<void(bool)> callback, const float ratio = 1.0f);
    void clearDirectionsQueue();
    void moveObject(const vector<Direction>& directions, function<void()> onMoved) const;
    
    // 自分のRectの指定されたトリガーのイベントを実行
    void runRectEventByTrigger(const Trigger trigger);
    
    // 地形
    TerrainObject* getTerrain(const vector<Direction>& directions = {});
    
    void reaction(function<void()> callback = nullptr);
    
    // インターフェース
    virtual void onEnterMap();                               // マップに追加された時
    virtual void onExitMap();                                // マップから削除された時
    virtual void onSearched(MapObject* mainChara) {};        // 調べられた時
    virtual void onEventStart() {};                          // イベント開始時
    virtual void onEventFinished() {};                       // イベント終了時

    void drawDebugMask(); // デバッグ用マスク
    void drawDebugCollisionMask();
    void drawDebugInfo();
    
    friend class MovePattern;
};

#endif // __MAP_OBJECT_H__
