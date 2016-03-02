//
//  MapObjectFactory.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/02.
//
//

#ifndef __LastSupper__MapObjectFactory__
#define __LastSupper__MapObjectFactory__

#include "Common.h"

#include "MapObjects/PathObject.h"

class MapObject;
class MapObjectList;

class MapObjectFactory : public Ref
{
// 定数
public:
    enum struct Group
    {
        COLLISION,
        EVENT,
        CHARACTER,
        TERRAIN,
        ITEM,
        GHOST,
        PATH,
        
        SIZE,
    };
    
// クラスメソッド
public:
    static MapObjectList* createMapObjectList(TMXTiledMap* tiledMap);
    
// インスタンス変数
private:
    TMXTiledMap* tiledMap { nullptr };
    
// インスタンスメソッド
private:
    MapObjectFactory();
    ~MapObjectFactory();
    Rect getRect(const ValueMap& info) const;
    int getObjectId(const ValueMap& info) const;
    int getEventId(const ValueMap& info) const;
    string getObjectType(const ValueMap& info) const;
    Trigger getTrigger(const ValueMap& info) const;
    int getCharacterId(const ValueMap& info) const;
    CharacterMovePattern getCharacterMovePattern(const ValueMap& info) const;
    Direction getDirection(const ValueMap& info) const;
    Point getGridPosition(const Rect& rect) const;
    Sprite* getSprite(const ValueMap& info) const;
    int getPathId(const ValueMap& info) const;
    int getPreviousPathId(const ValueMap& info) const;
    int getNextPathId(const ValueMap& info) const;
    bool isThroughObject(const ValueMap& info) const;
    float getSpeedRatio(const ValueMap& info) const;
    bool isHit(const ValueMap& info) const;
    
    MapObject* createObjectOnCollision(const ValueMap& info);
    MapObject* createObjectOnEvent(const ValueMap& info);
    MapObject* createObjectOnCharacter(const ValueMap& info);
    MapObject* createObjectOnTerrain(const ValueMap& info);
    MapObject* createObjectOnItem(const ValueMap& info);
    MapObject* createObjectOnGhost(const ValueMap& info);
    MapObject* createObjectOnPath(const ValueMap& info);
};

#endif /* defined(__LastSupper__MapObjectFactory__) */
