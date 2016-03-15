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

// コンストラクタ
MapObjectList::MapObjectList() {FUNCLOG};

// デストラクタ
MapObjectList::~MapObjectList()
{
    FUNCLOG

    this->availableObjects.clear();
    this->disableObjects.clear();
    this->enemies.clear();
    this->terrainObjects.clear();
    CC_SAFE_RELEASE_NULL(this->plainArea);
};

// 初期化
bool MapObjectList::init()
{
    // ノーマルの地形を生成
    PlainArea* plainArea { PlainArea::create() };
    CC_SAFE_RETAIN(plainArea);
    this->plainArea = plainArea;
    
    // 敵と主人公一行の衝突判定開始
    this->scheduleUpdate();
    
    return true;
}

// 有効オブジェクトリストを設定
void MapObjectList::setAvailableObjects(const Vector<MapObject*>& objects)
{
    if(!this->availableObjects.empty()) return;
    
    this->availableObjects = objects;
}

// 無効オブジェクトを設定
void MapObjectList::setDisableObjects(const Vector<MapObject*>& objects)
{
    if(!this->disableObjects.empty()) return;
    
    this->disableObjects = objects;
};

// 地形オブジェクトを設定
void MapObjectList::setTerrainObjects(const Vector<TerrainObject*>& objects)
{
    if(!this->terrainObjects.empty()) return;
    
    this->terrainObjects = objects;
};

// 経路オブジェクト
void MapObjectList::setPathObjects(const Vector<PathObject*>& objects)
{
    if(!this->pathObjects.empty()) return;
    
    this->pathObjects = objects;
}

// 指定IDのマップオブジェクトを取得
MapObject* MapObjectList::getMapObject(int objId) const
{
    for(MapObject* obj : this->availableObjects)
    {
        if(objId == obj->getObjectId())
        {
            return obj;
        }
    }
    
    return nullptr;
}

// 無効リストから指定IDのマップオブジェクトを取得
MapObject* MapObjectList::getMapObjectFromDisableList(int objId) const
{
    for(MapObject* obj : this->disableObjects)
    {
        if(objId == obj->getObjectId())
        {
            return obj;
        }
    }
    
    return nullptr;
}

// 有効マップオブジェクトのベクタを取得
const Vector<MapObject*>& MapObjectList::getMapObjects() const
{
    return this->availableObjects;
}

// 当たり判定を持つオブジェクトを取得
Vector<MapObject*> MapObjectList::getCollisionObjects(vector<MapObject*> exclusion) const
{
    Vector<MapObject*> collisionObjects {};
    
    for(MapObject* obj : this->getMapObjects())
    {
        if(!obj->isHit()) continue;
        
        bool flg { false };
        for(MapObject* ex : exclusion)
        {
            if(obj == ex) flg = true;
        }
        
        if(flg) continue;
        
        collisionObjects.pushBack(obj);
    }
    
    return collisionObjects;
}

// 指定範囲内にあるマップオブジェクトのベクタを取得
Vector<MapObject*> MapObjectList::getMapObjects(const Rect& rect) const
{
    Vector<MapObject*> mapObjects {};
    
    for(MapObject* obj : this->availableObjects)
    {
        if(rect.intersectsRect(obj->getCollisionRect())) mapObjects.pushBack(obj);
    }
    
    return mapObjects;
}

// 指定座標を含むマップオブジェクトのベクタを取得
Vector<MapObject*> MapObjectList::getMapObjects(const Point& position) const
{
    Vector<MapObject*> mapObjects {};
    for(MapObject* obj : this->availableObjects)
    {
        if(obj->getCollisionRect().containsPoint(position)) mapObjects.pushBack(obj);
    }
    
    return mapObjects;
}

// Rect(マス座標, マスサイズ)とトリガーからマップオブジェクトを取得
Vector<MapObject*> MapObjectList::getMapObjectsByGridRect(const Rect& gridRect, const Trigger trigger) const
{
    Vector<MapObject*> mapObjects {};
    bool needToCheckTrigger {trigger != Trigger::SIZE};
    
    for(MapObject* obj : this->availableObjects)
    {
        bool flag { false };
        
        Rect rect {obj->getGridRect()};
        
        if(MapUtils::intersectsGridRect(gridRect, rect))
        {
            if(!needToCheckTrigger) flag = true;
            
            if(obj->getTrigger() == trigger) flag = true;
        }
        
        if(flag) mapObjects.pushBack(obj);
    }
    
    return mapObjects;
}

// 指定トリガーのマップオブジェクトのEventIDベクタを取得
vector<int> MapObjectList::getEventIds(const Trigger trigger) const
{
    vector<int> eventIds {};
    
    for(MapObject* obj : this->availableObjects)
    {
        if(obj->getTrigger() == trigger) eventIds.push_back(obj->getEventId());
    }
    
    return eventIds;
}

// マスRectとTriggerからイベントIDを取得
vector<int> MapObjectList::getEventIdsByGridRect(const Rect& gridRect, const Trigger trigger) const
{
    vector<int> ids {};
    
    for(MapObject* obj : this->getMapObjectsByGridRect(gridRect, trigger))
    {
        if(obj->getEventId() == etoi(EventID::UNDIFINED)) continue;
        ids.push_back(obj->getEventId());
    }
    
    return ids;
}

// 当たり判定を持つオブジェクトのマスRectを全て取得(例外を指定できる)
vector<Rect> MapObjectList::getGridCollisionRects(MapObject* exclusion) const
{
    vector<Rect> collisionRects {this->getGridCollisionRects({exclusion})};
    
    return collisionRects;
}

// 当たり判定を持つオブジェクトのマスRectを全て取得(例外を指定できる)
vector<Rect> MapObjectList::getGridCollisionRects(vector<MapObject*> exclusion) const
{
    vector<Rect> gridRects {};
    
    for(MapObject* obj : this->getCollisionObjects(exclusion))
    {
        gridRects.push_back(obj->getGridRect());
    }
    
    return gridRects;
}

// 当たり判定を持つオブジェクトのCollisionRectを例外を除いて全て取得
vector<Rect> MapObjectList::getCollisionRects(vector<MapObject*> exclusion) const
{
    vector<Rect> collisionRects {};
    
    for(MapObject* obj : this->getCollisionObjects(exclusion))
    {
        collisionRects.push_back(obj->getCollisionRect());
    }
    
    return collisionRects;
}

// マップオブジェクトを追加
void MapObjectList::add(MapObject* mapObject)
{
    // もうすでにリスト内にあればリターン
    if(this->availableObjects.find(mapObject) != this->availableObjects.end()) return;
    
    this->availableObjects.pushBack(mapObject);
    
    std::mutex mtx;
    
    // 無効リストに存在する場合は削除
    for(MapObject* obj : this->disableObjects)
    {
        if(mapObject->getObjectId() == obj->getObjectId())
        {
            mtx.lock();
            this->disableObjects.eraseObject(mapObject);
            mtx.unlock();
            
            break;
        }
    }
}

// マップオブジェクトを削除
void MapObjectList::removeById(const int objectId)
{
    mutex mtx;
    
    for(MapObject* obj : this->availableObjects)
    {
        if(obj->getObjectId() == objectId)
        {
            mtx.lock();
            obj->removeFromParent();
            this->disableObjects.pushBack(obj);
            this->availableObjects.eraseObject(obj);
            mtx.unlock();
        }
    }
}

#pragma mark -
#pragma mark Enemy

// 敵を追加
void MapObjectList::addEnemy(Enemy* enemy)
{
    this->enemies.pushBack(enemy);
}

// 敵を削除
void MapObjectList::removeEnemyById(const int enemyId)
{
    std::mutex mtx;
    
    for(Enemy* enemy : this->enemies)
    {
        if(enemy->getEnemyId() == enemyId)
        {
            mtx.lock();
            enemy->removeFromParent();
            this->enemies.eraseObject(enemy);
            mtx.unlock();
        }
    }
}

// 敵を全て取得
Vector<Enemy*> MapObjectList::getEnemiesAll()
{
    return this->enemies;
}

// 敵が存在するか
bool MapObjectList::existsEnemy() const
{
    return !this->enemies.empty();
}

#pragma mark -
#pragma mark Party

// 主人公一行を格納
void MapObjectList::setParty(Party* party)
{
    // 一行が動いた時のコールバックを設定
    party->onPartyMoved = CC_CALLBACK_1(MapObjectList::onPartyMoved, this);
    
    this->party = party;
}

// 主人公一行を取得
Party* MapObjectList::getParty()
{
    return this->party;
}

// 主人公一行が移動した時
void MapObjectList::onPartyMoved(const Rect& gridRect)
{
    // 敵に移動したことを通知する
    for(Enemy* enemy : this->enemies)
    {
        enemy->onPartyMoved();
    }
}

#pragma mark -
#pragma mark TerrainObject

// 地形オブジェクトをマスRectから取得
TerrainObject* MapObjectList::getTerrainByGridRect(const Rect& gridRect)
{
    for(TerrainObject* obj : this->terrainObjects)
    {
        if(!MapUtils::includesGridRect(obj->getGridRect(), gridRect)) continue;
        
        return obj;
    }
    
    // 何も見つからなかった場合はノーマル地形を返す
    return this->plainArea;
}

#pragma mark -
#pragma mark PathObject

// 経路オブジェクトをIDから取得
PathObject* MapObjectList::getPathObjectById(const int pathId)
{
    for(PathObject* obj : this->pathObjects)
    {
        if(obj->getPathId() == pathId) return obj;
    }
    
    return nullptr;
}

#pragma mark -
#pragma mark Event

// 全オブジェクトにイベント開始を通知
void MapObjectList::onEventStart()
{
    for(MapObject* obj : this->availableObjects)
    {
        obj->onEventStart();
    }
    
    for(Enemy* enemy : this->enemies)
    {
        enemy->onEventStart();
    }
}

// 全オブジェクトにイベント終了を通知
void MapObjectList::onEventFinished()
{
    for(MapObject* obj : this->availableObjects)
    {
        obj->onEventFinished();
    }
    
    for(Enemy* enemy : this->enemies)
    {
        enemy->onEventFinished();
    }
}

#pragma mark -
#pragma mark update

// 敵と主人公一行の衝突監視用updateメソッド
void MapObjectList::update(float delta)
{
    // partyがnullptrまたは、敵が一人もいない時は処理を中止
    if(!this->party || this->enemies.empty()) return;
    
    for(MapObject* obj : this->enemies)
    {
        // 主人公と敵が一体でもぶつかっていれば、コールバック呼び出し
        if(obj->getCollisionRect().intersectsRect(party->getMainCharacter()->getCollisionRect()))
        {
            // スケジュール終了
            this->unscheduleUpdate();
            if(!this->onContactWithEnemy) return;
            this->onContactWithEnemy();
        }
    }
}
