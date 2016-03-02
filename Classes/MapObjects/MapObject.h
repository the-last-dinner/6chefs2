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

class MapObject : public Node
{
// 定数
public:
    static const float DURATION_MOVE_ONE_GRID;
    
// インスタンス変数
private:
    int objectId { static_cast<int>(ObjectID::UNDIFINED)};
    int eventId { static_cast<int>(EventID::UNDIFINED) };
	Trigger trigger {Trigger::SIZE};
	bool _isHit { false };
    Rect collisionRect {Rect::ZERO};
	Light* light { nullptr };
    bool _isMoving { false };
    Sprite* sprite { nullptr };
    Vector<SpriteFrame*> spriteFrames {};
protected:
    MapObjectList* objectList { nullptr };
    deque<vector<Direction>> directionsQueue {};
    Location location {};
public:
    function<void(MapObject*)> onMoved { nullptr };
	
// インスタンスメソッド
public:
	MapObject();
	~MapObject();
	void setGridPosition(const Point& gridPosition);
    virtual void setDirection(const Direction direction);
    void setObjectId(int objectId);
	void setEventId(int eventId);
	void setTrigger(Trigger trigger);
	void setHit(bool _isHit);
    void setCollisionRect(const Rect& rect);
    void setMapObjectList(MapObjectList* objectList);
    void setSprite(Sprite* sprite);
    void addSpriteFrame(SpriteFrame* spriteFrame);
    
	void setLight(Light* light, AmbientLightLayer* ambientLightLayer, function<void()> callback = nullptr);
	void removeLight(function<void()> callback = nullptr);
	
    Location getLocation() const;
    Size  getGridSize() const;
	Point getGridPosition() const;
    Rect getGridRect(const vector<Direction>& directions = {}) const;
    int getObjectId() const;
	int getEventId() const;
	Trigger getTrigger() const;
    bool isMoving() const;
    Direction getDirection() const;
    Sprite* getSprite() const;
    Vector<SpriteFrame*> getSpriteFrames() const;
    
    // collision
    virtual vector<Rect> getWorldGridCollisionRects();
    Rect getCollisionRect() const;
    Rect getCollisionRect(const Direction& direction) const;
    Rect getCollisionRect(const vector<Direction>& directions) const;
    const bool isHit() const;
    const bool isHit(const Direction& direction) const;
    virtual const bool isHit(const vector<Direction>& directions) const;
    
    // move
    vector<Direction> createEnableDirections(const vector<Direction>& directions) const;
    Vec2 createMoveVec(const vector<Direction>& directions, const bool check = true) const;
    bool canMove(const vector<Direction>& directions) const;
    void move(const vector<Direction>& enableDirections, function<void()> onMoved, const float ratio = 1.0f);
    bool moveBy(const Direction& direction, function<void()> onMoved, const float ratio = 1.0f);
    bool moveBy(const vector<Direction>& directions, function<void()> onMoved, const float ratio = 1.0f);
    void moveBy(const Direction& direction, const int gridNum, function<void(bool)> onMoved, const float ratio = 1.0f);
    void moveBy(const vector<Direction>& directions, const int gridNum, function<void(bool)> onMoved, const float ratio = 1.0f);
    void moveByQueue(deque<Direction> directionsQueue, function<void(bool)> callback, const float ratio = 1.0f);
    void moveByQueue(deque<vector<Direction>> directionsQueue, function<void(bool)> callback, const float ratio = 1.0f);
    void clearDirectionsQueue();
    
    // 地形
    TerrainObject* getTerrain(const vector<Direction>& directions = {});
    
    void reaction(function<void()> callback = nullptr);
    
    // イベント関数
    virtual void onEnterMap() {};                               // マップに追加された時
    virtual void onSearched(MapObject* mainChara) {};           // 調べられた時
    
    // ポリモーフィック用
    virtual void moveStart() {};                                // 動き開始
    virtual void moveStop() {};                                 // 動き停止
    
    Direction convertToWorldDir(const Direction direcction);

    void drawDebugMask(); // デバッグ用マスク
    
    friend class MovePattern;
};

#endif // __MAP_OBJECT_H__
