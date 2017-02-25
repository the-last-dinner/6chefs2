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
class MapObjectCommand;
class MapObjectCommandQueue;
class HitBox;
class HitPoint;
class Battle;
class CSNode;

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
    string _movingSoundFileName {};
	Light* _light { nullptr };
    bool _isMoving { false };
    Sprite* _sprite { nullptr };
    bool _paused { false };
    int _getOffEventID { static_cast<int>(EventID::UNDIFINED)};
    int _rideOnEventID { static_cast<int>(EventID::UNDIFINED)};
    vector<Direction> _movingDirections {};
    CollisionBox* _collision { nullptr };
    Label* _debugLabel { nullptr };
    function<void(MapObject*)> _onLostHP { nullptr };
protected:
    MapObjectList* _objectList { nullptr };
    Location _location {};
    TerrainState* _terrainState { nullptr };
    TerrainStateCache* _terrainStateCache { nullptr };
    MapObjectCommandQueue* _commandQueue { nullptr };
    HitBox* _hitBox { nullptr };
    HitPoint* _hitPoint { nullptr };
    Battle* _battle { nullptr };
    CSNode* _csNode { nullptr };
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
    void setMovingSoundFileName(const string& fileName);
    void setMapObjectList(MapObjectList* objectList);
    void setSprite(Sprite* sprite);
    void setPaused(bool paused);
    void setCollision(CollisionBox* collision);
    
	void setLight(Light* innerLight, Light* outerLight, AmbientLightLayer* ambientLightLayer, function<void()> callback = nullptr);
	void removeLight(function<void()> callback = nullptr);
	
    Location getLocation() const { return _location; }
    Size  getGridSize() const;
	Point getGridPosition() const;
    Rect getGridRect(const vector<Direction>& directions = {}) const;
    Rect getGridCollisionRect(const vector<Direction>& directions = {}) const;
    int getObjectId() const { return _objectId; }
    int getEventId() const { return _eventId; }
    Trigger getTrigger() const { return _trigger; }
    bool isMoving() const { return _isMoving; }
    Direction getDirection() const { return _location.direction; }
    Sprite* getSprite() const { return _sprite; }
    bool isPaused() const { return _paused; }
    vector<Direction> getMovingDirections() const { return _movingDirections; }
    CollisionBox* getCollision() const { return _collision; }
    bool isChangeableDirection(const Direction& direction);
    Battle* getBattle() const { return _battle; }
    
// collision
public:
    Rect getCollisionRect() const;
    bool isHit() const { return _collision; }
    bool isHit(const Direction& direction, bool ignoreCollision = false) const;
    virtual bool isHit(const vector<Direction>& directions, bool ignoreCollision = false) const;
    virtual bool isHit(const MapObject* other) const;
    bool isMovable() const { return _isMovable; }
    string getMovingSoundFileName() const { return _movingSoundFileName; }
    Vector<MapObject*> getHitObjects(const Direction& direction) const;
    Vector<MapObject*> getHitObjects(const vector<Direction>& directions) const;
    
// hit
public:
    void enableHit(bool enable);
    
// command
public:
    void pushCommand(MapObjectCommand* command);
    void clearCommandQueue();
private:
    void executeCommandFromQueue();
    
// HP
public:
    HitPoint* getHitPoint() const;
    void setLostHPCallback(function<void(MapObject*)> callback);
    virtual void onLostHP();
    
// Battle
public:
    virtual bool canAttack(MapObject* target) const;
    
// CSNode
public:
    void playAnimation(const string& name, float speed, bool loop = false);
    void playAnimationIfNotPlaying(const string& name, float speed = 1.f);
    void playAnimation(const string& name, function<void(MapObject*)> callback);

// move
public:
    vector<Direction> createEnableDirections(const vector<Direction>& directions, bool ignoreCollision = false) const;
    bool canMove(const vector<Direction>& directions) const;
    void move(const vector<Direction>& enableDirections, function<void()> onMoved, float speed);
    bool moveBy(const vector<Direction>& directions, function<void(bool)> cb, float speed, bool ignoreCollision);
    void moveObject(const vector<Direction>& directions, function<void(bool)> cb) const;
    
    // 自分のRectの指定されたトリガーのイベントを実行
    void runRectEventByTrigger(const Trigger trigger);
    
    // 地形
    TerrainObject* getTerrain(const vector<Direction>& directions = {});
    
    void reaction(function<void()> callback = nullptr);
    
// インターフェース
public:
    virtual void update(float delta);                        // 毎フレーム処理
    virtual void onEnterMap();                               // マップに追加された時
    virtual void onExitMap();                                // マップから削除された時
    virtual void onSearched(MapObject* mainChara) {};        // 調べられた時
    virtual void onEventStart() {};                          // イベント開始時
    virtual void onEventFinished() {};                       // イベント終了時
    virtual void onBattleStart(Battle* battle);              // バトル開始時
    virtual void onBattleFinished();                         // バトル終了時
    virtual void pauseAnimation();
    virtual void resumeAnimation();

// デバッグ
public:
    void drawDebugInfo();
    
    friend class MovePattern;
};

#endif // __MAP_OBJECT_H__
