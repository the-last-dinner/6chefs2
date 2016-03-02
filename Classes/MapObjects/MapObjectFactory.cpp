//
//  MapObjectFactory.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/02.
//
//

#include "MapObjects/MapObjectFactory.h"

#include "Datas/MapObject/CharacterData.h"

#include "MapObjects/Character.h"
#include "MapObjects/EventObject.h"
#include "MapOBjects/GhostObject.h"
#include "MapObjects/ItemObject.h"
#include "MapObjects/PathObject.h"
#include "MapObjects/MapObjectList.h"

#include "MapObjects/TerrainObject/WaterArea.h"
#include "MapObjects/TerrainObject/SlipFloor.h"

// コンストラクタ
MapObjectFactory::MapObjectFactory() {FUNCLOG};

// デストラクタ
MapObjectFactory::~MapObjectFactory() {FUNCLOG};

// MapObjectListを生成して返す
MapObjectList* MapObjectFactory::createMapObjectList(TMXTiledMap* tiledMap)
{
    if(!tiledMap) return nullptr;
    
    // 自身を生成
    MapObjectFactory* p {new(nothrow) MapObjectFactory()};
    
    p->tiledMap = tiledMap;
    
    // レイヤ別に処理が分かれるので用意
    map<MapObjectFactory::Group, string> typeToString
    {
        {Group::COLLISION, "collision"},
        {Group::EVENT, "event"},
        {Group::CHARACTER, "Chara(object)"},
        {Group::TERRAIN, "terrain"},
        {Group::ITEM, "item"},
        {Group::GHOST, "Ugokumono(object)"},
        {Group::PATH, "path"},
    };
    
    // グループごとに生成メソッドを用意
    map<Group, function<MapObject*(const ValueMap&)>> typeToFunc
    {
        {Group::COLLISION, CC_CALLBACK_1(MapObjectFactory::createObjectOnCollision, p)},
        {Group::EVENT, CC_CALLBACK_1(MapObjectFactory::createObjectOnEvent, p)},
        {Group::CHARACTER, CC_CALLBACK_1(MapObjectFactory::createObjectOnCharacter, p)},
        {Group::TERRAIN, CC_CALLBACK_1(MapObjectFactory::createObjectOnTerrain, p)},
        {Group::ITEM, CC_CALLBACK_1(MapObjectFactory::createObjectOnItem, p)},
        {Group::GHOST, CC_CALLBACK_1(MapObjectFactory::createObjectOnGhost, p)},
        {Group::PATH, CC_CALLBACK_1(MapObjectFactory::createObjectOnPath, p)},
    };
    
    // ベクタを用意
    Vector<MapObject*> availableObjects {};
    Vector<MapObject*> disableObjects {};
    Vector<TerrainObject*> terrainObjects {};
    Vector<PathObject*> pathObjects {};
    
    for(int i {0}; i < static_cast<int>(MapObjectFactory::Group::SIZE); i++)
    {
        Group group {static_cast<Group>(i)};
        
        TMXObjectGroup* objGroup { tiledMap->getObjectGroup(typeToString.at(group)) };
        if(!objGroup) continue;
        
        ValueVector infos {objGroup->getObjects()};
        for(cocos2d::Value info : infos)
        {
            MapObject* obj {typeToFunc[group](info.asValueMap())};
            if(!obj) continue;
            
            switch (group)
            {
                case Group::CHARACTER :
                case Group::GHOST :
                    disableObjects.pushBack(obj);
                    break;
                    
                case Group::PATH :
                    pathObjects.pushBack(dynamic_cast<PathObject*>(obj));
                    break;
                    
                case Group::TERRAIN :
                    terrainObjects.pushBack(dynamic_cast<TerrainObject*>(obj));
                    break;
                    
                default:
                    availableObjects.pushBack(obj);
                    break;
            }
        }
    }
    
    // delete
    delete p;
    
    // MapObjectListを生成して返す
    MapObjectList* list { MapObjectList::create() };
    
    list->setAvailableObjects(availableObjects);
    list->setDisableObjects(disableObjects);
    list->setTerrainObjects(terrainObjects);
    list->setPathObjects(pathObjects);
    
    return list;
}

// オブジェクトの位置、大きさを取得
Rect MapObjectFactory::getRect(const ValueMap& info) const
{
    return Rect(info.at("x").asInt(), info.at("y").asInt(), info.at("width").asInt(), info.at("height").asInt());
}

// オブジェクトIDを取得
int MapObjectFactory::getObjectId(const ValueMap& info) const
{
    return (info.count("objID") != 0)?info.at("objID").asInt():static_cast<int>(ObjectID::UNDIFINED);
}

// イベントIDを取得
int MapObjectFactory::getEventId(const ValueMap& info) const
{
    return (info.count("EventID") != 0)?info.at("EventID").asInt():static_cast<int>(EventID::UNDIFINED);
}

// オブジェクトの種類名を取得
string MapObjectFactory::getObjectType(const ValueMap& info) const
{
    return info.at("type").asString();
}

// トリガーを取得
Trigger MapObjectFactory::getTrigger(const ValueMap& info) const
{
    return (info.count("trigger") != 0)?static_cast<Trigger>(info.at("trigger").asInt()):Trigger::SIZE;
}

// キャラクターIDを取得
int MapObjectFactory::getCharacterId(const ValueMap& info) const
{
    if(info.count("charaID") != 0)
    {
        return info.at("charaID").asInt();
    }
    else if(info.count("CharaID") != 0)
    {
        return info.at("CharaID").asInt();
    }
    
    return static_cast<int>(CharacterID::UNDIFINED);
}

// キャラクターの動き方を取得
CharacterMovePattern MapObjectFactory::getCharacterMovePattern(const ValueMap& info) const
{
    return (info.count("movePattern") != 0)?static_cast<CharacterMovePattern>(info.at("movePattern").asInt()):CharacterMovePattern::SIZE;
}

// 向きを取得
Direction MapObjectFactory::getDirection(const ValueMap& info) const
{
    if(info.count("direction") == 0) return Direction::SIZE;
    
    return MapUtils::toEnumDirection(info.at("direction").asString());
}

// マス座標を取得
Point MapObjectFactory::getGridPosition(const Rect& rect) const
{
    return MapUtils::convertToMapPoint(this->tiledMap->getContentSize(), Point(rect.getMinX(), rect.getMinY())) / GRID;
}

// Spriteを取得
Sprite* MapObjectFactory::getSprite(const ValueMap& info) const
{
    if(info.count("img") == 0) return nullptr;
    
    return Sprite::createWithSpriteFrameName(info.at("img").asString());
}

// 経路オブジェクトIDを取得
int MapObjectFactory::getPathId(const ValueMap& info) const
{
    if(info.count("pathID") == 0) return -1;
    
    return info.at("pathID").asInt();
}

// 前の経路オブジェクトIDを取得
int MapObjectFactory::getPreviousPathId(const ValueMap& info) const
{
    if(info.count("previousID") == 0) return -1;
    
    return info.at("previousID").asInt();
}

// 次の経路オブジェクトIDを取得
int MapObjectFactory::getNextPathId(const ValueMap& info) const
{
    if(info.count("nextID") == 0) return -1;
    
    return info.at("nextID").asInt();
}

// 周りを見渡す必要があるか
bool MapObjectFactory::isThroughObject(const ValueMap& info) const
{
    if(info.count("through") == 0) return false;
    
    return info.at("through").asBool();
}

// 速度倍率を取得
float MapObjectFactory::getSpeedRatio(const ValueMap& info) const
{
    if(info.count("speed") == 0) return 1.f;
    
    return info.at("speed").asFloat();
}

// 当たり判定があるかどうか
bool MapObjectFactory::isHit(const ValueMap &info) const
{
    if(info.count("hit") == 0) return false;
    
    return info.at("hit").asBool();
}

// 当たり判定レイヤにあるオブジェクトを生成
MapObject* MapObjectFactory::createObjectOnCollision(const ValueMap& info)
{
    Rect rect {this->getRect(info)};
    
    EventObject* pObj { EventObject::create() };
    pObj->setObjectId(this->getObjectId(info));
    pObj->setGridPosition(this->getGridPosition(rect));
    pObj->setContentSize(rect.size);
    pObj->setHit(true);
    pObj->setCollisionRect(Rect(0, 0, rect.size.width, rect.size.height));
    
    return pObj;
}

// イベントレイヤにあるオブジェクトを生成
MapObject* MapObjectFactory::createObjectOnEvent(const ValueMap& info)
{
    Rect rect {this->getRect(info)};

    EventObject* obj { EventObject::create() };
    obj->setObjectId(this->getObjectId(info));
    obj->setEventId(this->getEventId(info));
    obj->setTrigger(this->getTrigger(info));
    obj->setGridPosition(this->getGridPosition(rect));
    obj->setContentSize(rect.size);
    obj->setCollisionRect(Rect(0, 0, rect.size.width, rect.size.height));
    obj->setHit(this->isHit(info));
    obj->setSprite(this->getSprite(info));
    
    return obj;
}

// キャラクターレイヤにあるオブジェクトを生成
MapObject* MapObjectFactory::createObjectOnCharacter(const ValueMap& info)
{
    Rect rect {this->getRect(info)};
    Point gridPosition { this->getGridPosition(rect) };
    
    // データ生成
    CharacterData data;
    data.obj_id = this->getObjectId(info);
    data.chara_id = this->getCharacterId(info);
    data.location.x = gridPosition.x;
    data.location.y = gridPosition.y;
    data.location.direction = this->getDirection(info);
    data.move_pattern = this->getCharacterMovePattern(info);
    
    // データからキャラクタを生成
    Character* chara { Character::create(data) };
    
    if(!chara) return nullptr;
    
    chara->setTrigger(this->getTrigger(info));
    chara->setEventId(this->getEventId(info));
    chara->setHit(true);
    
    return chara;
}

// 地形レイヤにあるオブジェクトを生成
MapObject* MapObjectFactory::createObjectOnTerrain(const ValueMap& info)
{
    map<string, function<TerrainObject*()>> typeStrToFunc
    {
        {"slip", SlipFloor::create},
        {"water", WaterArea::create},
    };
    
    string typeStr {this->getObjectType(info)};
    
    if(typeStrToFunc.count(typeStr) == 0) return nullptr;
    
    Rect rect {this->getRect(info)};
    Point gridPosition { this->getGridPosition(rect) };
    
    TerrainObject* obj { typeStrToFunc[typeStr]() };
    
    if(!obj) return nullptr;
    
    obj->setObjectId(this->getObjectId(info));
    obj->setGridPosition(this->getGridPosition(rect));
    obj->setContentSize(rect.size);
    obj->setCollisionRect(Rect(0, 0, rect.size.width, rect.size.height));
    
    return obj;
}

// アイテムレイヤにあるオブジェクトを生成
MapObject* MapObjectFactory::createObjectOnItem(const ValueMap& info)
{
    ItemObject* obj { ItemObject::create() };
    
    if(!obj) return nullptr;
    
    obj->setObjectId(this->getObjectId(info));
    obj->setEventId(this->getEventId(info));
    obj->setTrigger(this->getTrigger(info));
    obj->setGridPosition(this->getGridPosition(this->getRect(info)));
    
    return obj;
}

// 動くものレイヤにあるオブジェクトを生成
MapObject* MapObjectFactory::createObjectOnGhost(const ValueMap& info)
{
    GhostObject* obj { GhostObject::create() };
    
    if(!obj) return nullptr;
    
    obj->setObjectId(this->getObjectId(info));
    obj->setEventId(this->getEventId(info));
    obj->setTrigger(this->getTrigger(info));
    obj->setGridPosition(this->getGridPosition(this->getRect(info)));
    Sprite* sprite { this->getSprite(info) };
    obj->setSprite(sprite);
    obj->setContentSize(sprite->getContentSize());
    obj->setCollisionRect(Rect(0, 0, sprite->getContentSize().width, sprite->getContentSize().height));
    
    return obj;
}

// 経路レイヤにあるオブジェクトを生成
MapObject* MapObjectFactory::createObjectOnPath(const ValueMap& info)
{
    PathObject* obj { PathObject::create() };
    
    if(!obj) return nullptr;
    
    obj->setObjectId(this->getObjectId(info));
    obj->setEventId(this->getEventId(info));
    obj->setTrigger(this->getTrigger(info));
    obj->setGridPosition(this->getGridPosition(this->getRect(info)));
    obj->setPathId(this->getPathId(info));
    obj->setPreviousId(this->getPreviousPathId(info));
    obj->setNextId(this->getNextPathId(info));
    obj->setThrough(this->isThroughObject(info));
    obj->setSpeedRatio(this->getSpeedRatio(info));
    obj->setLookDirection(this->getDirection(info));
    
    return obj;
}
