//
//  MapObject.cpp
//  LastSupper
//
//  Created by Kohei on 2015/06/21.
//
//

#include "MapObjects/MapObject.h"

#include "MapObjects/Command/MapObjectCommand.h"
#include "MapObjects/Command/MapObjectCommandQueue.h"
#include "MapObjects/Command/MoveCommand.h"
#include "MapObjects/DetectionBox/CollisionDetector.h"
#include "MapObjects/MapObjectList.h"
#include "MapObjects/TerrainState/TerrainState.h"
#include "MapObjects/TerrainState/TerrainStateCache.h"

#include "Effects/AmbientLightLayer.h"
#include "Effects/Light.h"

#include "Managers/DungeonSceneManager.h"

// １マス動くのにかける時間の基準値
const float MapObject::DURATION_MOVE_ONE_GRID = 0.1f;

// コンストラクタ
MapObject::MapObject() {}

// デストラクタ
MapObject::~MapObject()
{
    CC_SAFE_RELEASE_NULL(_terrainStateCache);
    CC_SAFE_RELEASE_NULL(_commandQueue);
}

// 初期化
bool MapObject::init()
{
    if(!Node::init()) return false;
    
    TerrainStateCache* terrainStateCache { TerrainStateCache::create() };
    CC_SAFE_RETAIN(terrainStateCache);
    _terrainStateCache = terrainStateCache;
    
    MapObjectCommandQueue* commandQueue { MapObjectCommandQueue::create() };
    CC_SAFE_RETAIN(commandQueue);
    _commandQueue = commandQueue;
    
    return true;
}

// マス数サイズを取得
Size MapObject::getGridSize() const { return Size(floor(this->getContentSize().width / GRID),floor(this->getContentSize().height / GRID)); }

// マップ上のマス座標を取得(一番左下のマス座標を返す)
Point MapObject::getGridPosition() const { return Point(_location.x, _location.y); }

// マスRectを取得
Rect MapObject::getGridRect(const vector<Direction>& directions) const
{
    Vec2 gridVec { Direction::getGridVec2(directions) };
    Rect gridRect { Rect::ZERO };
    
    gridRect.origin = this->getGridPosition();
    gridRect.size = this->getGridSize();
    
    return gridRect;
}

// マスあたり判定Rectを取得
Rect MapObject::getGridCollisionRect(const vector<Direction>& directions) const
{
    if(!_collision) return Rect::ZERO;
    
    return _collision->getGridRect(directions);
}

// マス座標をセット、実際の位置は変更しない
void MapObject::setGridPosition(const Point& gridPosition) { _location.x = gridPosition.x; _location.y = gridPosition.y; }

// 方向をセット
void MapObject::setDirection(const Direction& direction) { _location.direction = direction; }

// オブジェクトIDをセット
void MapObject::setObjectId(int objectId) { _objectId = objectId; }

// イベントIDをセット
void MapObject::setEventId(int eventId) { _eventId = eventId; }

// イベントのtriggerをセット
void MapObject::setTrigger(Trigger trigger) { _trigger = trigger; }

// かいりきで押せるかをセット
void MapObject::setMovable(bool isMovable) { _isMovable = isMovable; }

// マップオブジェクトのリストをセット
void MapObject::setMapObjectList(MapObjectList* objectList) { _objectList = objectList; }

// 当たり判定をセット
void MapObject::setCollision(CollisionBox* collision)
{
    if(_collision) return;
    if(!collision) return;
    
    this->addChild(collision);
    _collision = collision;
}

// Spriteを設定
void MapObject::setSprite(Sprite* sprite)
{
    if(!sprite) return;
    _sprite = sprite;
    this->addChild(sprite);
};

// 一時停止状態を設定
void MapObject::setPaused(bool paused) { _paused = paused; }

// ライトをセット
void MapObject::setLight(Light* light, AmbientLightLayer* ambientLightLayer, function<void()> callback)
{
    if(_light) return;
    
    // ライトを追加
    _light = light;
    this->addChild(light);
    light->setOpacity(0);
    light->setBlendFunc(BlendFunc{ GL_SRC_ALPHA, GL_ONE });
    light->runAction(Sequence::createWithTwoActions(FadeIn::create(0.5f), CallFunc::create(callback)));
    
    // 環境光レイヤーに光源として追加
    ambientLightLayer->addLightSource(light);
}

// ライトを消す
void MapObject::removeLight(function<void()> callback)
{
    if(!_light) return;
    
    Light* light { _light };
    _light = nullptr;
    
    light->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create(callback), RemoveSelf::create(), nullptr));
}

// Locationを取得
Location MapObject::getLocation() const {return _location;}

// オブジェクトIDを取得
int MapObject::getObjectId() const {return _objectId;}

// イベントIDを取得
int MapObject::getEventId() const {return _eventId;}

// triggerを取得
Trigger MapObject::getTrigger() const {return _trigger;}

// 移動中かどうか
bool MapObject::isMoving() const {return _isMoving;}

// 現在キャラが向いている方向を取得
Direction MapObject::getDirection() const {return _location.direction;}

// Spriteを取得
Sprite* MapObject::getSprite() const { return _sprite;};

// 一時停止状態か
bool MapObject::isPaused() const { return _paused; };

// 移動方向を取得
vector<Direction> MapObject::getMovingDirections() const { return _movingDirections; };

// 当たり判定を取得
CollisionBox* MapObject::getCollision() const { return _collision; }

// 衝突判定用Rectを取得
Rect MapObject::getCollisionRect() const
{
    if (!_collision) return Rect::ZERO;
    
    return _collision->getRect();
}

// 当たり判定の有無を取得
bool MapObject::isHit() const {return _collision;}

// 指定の方向に対して当たり判定があるか
bool MapObject::isHit(const Direction& direction, bool ignoreCollision) const
{
    return this->isHit(vector<Direction>{ direction }, ignoreCollision);
}

// 指定の２方向に対して当たり判定があるか
bool MapObject::isHit(const vector<Direction>& directions, bool ignoreCollision) const
{
    if (!_objectList) return false;
    
    bool intersects { false };
    
    if (ignoreCollision) {
        intersects = _objectList->getCollisionDetector()->intersectsExceptIgnorable(this, directions);
    } else {
        intersects = _objectList->getCollisionDetector()->intersects(this, directions);
    }
    
    return intersects;
}

// 指定のMapObjectに対して当たり判定があるか
bool MapObject::isHit(const MapObject* other) const
{
    return other;
}

// 動かせるかどうかを取得
bool MapObject::isMovable() const {return _isMovable;}

// 指定の方向に対して当たり判定があるMapObjectのvectorを取得
Vector<MapObject*> MapObject::getHitObjects(const Direction& direction) const
{
    return this->getHitObjects(vector<Direction>{ direction });
}

// 指定の２方向に対して当たり判定があるMapObjectのvectorを取得
Vector<MapObject*> MapObject::getHitObjects(const vector<Direction>& directions) const
{
    Vector<MapObject*> mapObjects {};
    
    if(!_objectList) return mapObjects;
    
    // 自身以外の当たり判定を持つオブジェクトが、指定方向にあればlistに入れる
    for(MapObject* obj : _objectList->getMapObjects(this, directions))
    {
        if(obj->isHit() && obj != this) mapObjects.pushBack(obj);
    }
    
    return mapObjects;
}

#pragma mark -
#pragma mark Command

void MapObject::pushCommand(MapObjectCommand* command)
{
    _commandQueue->push(command);
}

void MapObject::clearCommandQueue()
{
    _commandQueue->clear();
}

#pragma mark -
#pragma mark Move

// 入力のあった方向から、移動可能方向のみを取り出して返す
vector<Direction> MapObject::createEnableDirections(const vector<Direction>& directions, bool ignoreCollision) const
{
    vector<Direction> enableDirs {};
    
    // 入力が複数か
    bool isInputMultiple {directions.size() >= 2};
    
    // 入力が２以上の時、斜め方向に当たり判定があるか確認
    bool isHitDiagnally { isInputMultiple ? this->isHit(directions, ignoreCollision) : false };
    
    // 当たり判定
    for (Direction direction : directions) {
        if ((isInputMultiple && !isHitDiagnally) ||
            (!isHitDiagnally && !this->isHit(direction, ignoreCollision)) ||
            (isHitDiagnally && !this->isHit(direction, ignoreCollision) &&
             enableDirs.empty())) {
            enableDirs.push_back(direction);
        }
    }
    
    return enableDirs;
}

// 入力のあった方向の当たっているものを動かす
void MapObject::moveObject(const vector<Direction>& directions, function<void(bool)> onMoved) const
{
    // 入力が複数なら動かさない
    if (directions.size() >= 2) return;
    
    // 自分自身が動かせるオブジェクトなら動かさない
    if (this->isMovable()) return;
    
    // 当たり判定
    for (Direction direction : directions) {
        // 当たったものが動かせるなら入力方向に1マス動かす
        if (this->isHit(direction)) {
            for (MapObject* obj : this->getHitObjects(direction)) {
                if(obj->isMovable()) obj->moveBy({ direction }, [onMoved](bool movable) {
                    // 複数の物体が同時に接触した時に例外が出るのでキャッチ
                    try {
                        onMoved(movable);
                    } catch(exception e) {
                    }
                    DungeonSceneManager::getInstance()->runEventQueue();
                }, 1.f, false);
            }
        }
    }
}

// 入力方向に対して動くことが可能かどうか
bool MapObject::canMove(const vector<Direction>& directions) const { return !this->createEnableDirections(directions).empty(); }

// 当たり判定、地形チェックをせずにして方向に移動させる
void MapObject::move(const vector<Direction>& enableDirections, function<void()> onMoved, float speed)
{
    // 移動中の方向を設定
    _movingDirections = enableDirections;
    
    // 方向からベクトル生成
    Vec2 movement { Direction::getVec2(enableDirections) };
    
    // マス座標を変更
    this->setGridPosition(this->getGridPosition() + Vec2(movement.x, -movement.y) / GRID);
    
    // 移動開始
    this->_isMoving = true;
    this->runAction(Sequence::createWithTwoActions(MoveBy::create(DURATION_MOVE_ONE_GRID / speed, movement), CallFunc::create([this, onMoved] {
        this->_isMoving = false;
        if (this->onMoved) this->onMoved(this);
        if (onMoved) onMoved();
        _movingDirections.clear();
    })));
}

// 方向指定移動メソッド
bool MapObject::moveBy(const vector<Direction>& directions, function<void(bool)> cb, float speed, bool ignoreCollision)
{
    // 移動可能な方向を生成
    vector<Direction> dirs { this->createEnableDirections(directions, ignoreCollision) };
    
    // 押せるものがあれば押す
    this->moveObject(directions, cb);
    
    // 移動可能な方向がなければ失敗としてリターン + 失敗をコールバック
    if (dirs.empty()) {
        cb(false);
        return false;
    }
    
    // 移動する前の場所のイベントを発火
    this->runRectEventByTrigger(Trigger::GET_OFF);
    
    // 地形の状態
    _terrainState = this->getTerrain(dirs)->getTerrainState(_terrainStateCache);
    if (_terrainState) _terrainState->move(this, dirs, [cb] { cb(true); }, speed);
    
    // 移動したあとのイベントを発火
    this->runRectEventByTrigger(Trigger::RIDE_ON);
    
    return true;
}

// 指定方向にある地形オブジェクトを取得
TerrainObject* MapObject::getTerrain(const vector<Direction>& directions)
{
    if (!_objectList) return nullptr;

    return _objectList->getTerrain(this, directions);
}

// 自分のRectの指定されたトリガーのイベントを実行
void MapObject::runRectEventByTrigger(const Trigger trigger)
{
    Vector<MapObject*> rideOnMapObjects { _objectList->getMapObjects(this, trigger) };
    int* eventID;
    switch (trigger) {
        case Trigger::RIDE_ON:
            eventID = &_rideOnEventID;
            break;
        case Trigger::GET_OFF:
            eventID = &_getOffEventID;
            break;
        default:
            break;
    }
    if (rideOnMapObjects.empty()) *eventID = etoi(EventID::UNDIFINED);
    
    for(MapObject* obj : rideOnMapObjects) {
        if (obj->getEventId() != *eventID) {
            if (*eventID == etoi(EventID::UNDIFINED)) *eventID = obj->getEventId();
            DungeonSceneManager::getInstance()->pushEventFront(obj->getEventId());
        }
    }
}

// リアクション
void MapObject::reaction(function<void()> callback)
{
    Sprite* icon {Sprite::createWithSpriteFrameName("icon_sign.png")};
    icon->setPosition(Point(0, this->getContentSize().height / 2));
    icon->setGlobalZOrder(Priority::TOP_COVER);
    icon->setScaleY(0.01f);
    icon->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    this->addChild(icon);
    
    icon->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.6f, 1.f), 0.5f), DelayTime::create(1.f), RemoveSelf::create(), CallFunc::create(callback), nullptr));
}

#pragma mark -
#pragma mark Debug

// デバッグ用に枠を描画
void MapObject::drawDebugMask()
{
    Point vertices[]
    {
        Point::ZERO,
        Point(0, this->getContentSize().height),
        this->getContentSize(),
        Point(this->getContentSize().width, 0),
        Point::ZERO,
    };
    Color4F lineColor = Color4F::BLUE;
    DrawNode* draw {DrawNode::create()};
    draw->drawPolygon(vertices, 5, Color4F(0,0,0,0), 1, lineColor);
    draw->setPosition(this->getContentSize() / -2);
    draw->setGlobalZOrder(Priority::DEBUG_MASK);
    this->addChild(draw);
    
    this->drawDebugCollisionMask();
}

void MapObject::drawDebugCollisionMask()
{
    if (!_collision) return;
    
    Rect collisionRect { this->getCollisionRect() };
    
    Point vertices[]
    {
        Point::ZERO,
        Point(0, collisionRect.size.height),
        collisionRect.size,
        Point(collisionRect.size.width, 0),
        Point::ZERO
    };
    Color4F lineColor { Color4F::GREEN };
    DrawNode* draw { DrawNode::create() };
    draw->drawPolygon(vertices, 5, Color4F(0,0,0,0), 1, lineColor);
    draw->setPosition(this->getContentSize() / -2);
    draw->setGlobalZOrder(Priority::DEBUG_MASK);
    this->addChild(draw);
}

void MapObject::drawDebugInfo()
{
    if (!_collision) return;
    
    if (_debugLabel) {
        _debugLabel->removeFromParent();
        _debugLabel = nullptr;
    }
    
    string labelStr { "" };
    labelStr += "grect  : (" + to_string(etoi(this->getGridPosition().x)) + ", " + to_string(etoi(this->getGridPosition().y));
    labelStr += ", " + to_string(etoi(this->getGridSize().width)) + ", " + to_string(etoi(this->getGridSize().height)) + ")\n";
    labelStr += "cgrect : (" + to_string(etoi(this->getGridCollisionRect().origin.x)) + ", " + to_string(etoi(this->getGridCollisionRect().origin.y));
    labelStr += ", " + to_string(etoi(this->getGridCollisionRect().size.width)) + ", " + to_string(etoi(this->getGridCollisionRect().size.height)) + ")";
    
    Label* label { Label::createWithTTF(labelStr, Resource::Font::CONFIG, 11) };
    label->setGlobalZOrder(Priority::DEBUG_MASK);
    label->setPosition(0, this->getContentSize().height / 2 + 13);
    this->addChild(label);
    _debugLabel = label;
}

#pragma mark -
#pragma mark Interface

void MapObject::update(float delta)
{
    _commandQueue->update(this, delta);
}

void MapObject::onEnterMap()
{
    this->scheduleUpdate();
    
    if (_objectList && _collision) {
        _objectList->getCollisionDetector()->addCollision(_collision);
    }
}

void MapObject::onExitMap()
{
    if (_objectList && _collision) {
        _objectList->getCollisionDetector()->removeCollision(_collision);
    }
    
    this->unscheduleUpdate();
}
