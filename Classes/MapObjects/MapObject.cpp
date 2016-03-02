//
//  MapObject.cpp
//  LastSupper
//
//  Created by Kohei on 2015/06/21.
//
//

#include "MapObjects/MapObject.h"

#include "MapObjects/MapObjectList.h"

#include "Effects/AmbientLightLayer.h"
#include "Effects/Light.h"

// １マス動くのにかける時間の基準値
const float MapObject::DURATION_MOVE_ONE_GRID = 0.1f;

// コンストラクタ
MapObject::MapObject(){FUNCLOG}

// デストラクタ
MapObject::~MapObject(){FUNCLOG}

// マス数サイズを取得
Size MapObject::getGridSize() const { return Size(floor(this->getContentSize().width / GRID),floor(this->getContentSize().height / GRID)); }

// マップ上のマス座標を取得(一番左下のマス座標を返す)
Point MapObject::getGridPosition() const { return Point(this->location.x, this->location.y); }

// マス座標、マスあたり判定サイズのRectを取得
Rect MapObject::getGridRect(const vector<Direction>& directions) const
{
    Vec2 movement { this->createMoveVec(directions, false) };
    movement = Vec2(movement.x, -movement.y) / GRID;
    
    return Rect(this->getGridPosition().x + this->collisionRect.getMinX() / GRID + movement.x, this->getGridPosition().y + this->collisionRect.getMinY() / GRID + movement.y, this->collisionRect.size.width / GRID, this->collisionRect.size.height / GRID);
}

// マス座標をセット、実際の位置は変更しない
void MapObject::setGridPosition(const Point& gridPosition) { this->location.x = gridPosition.x; this->location.y = gridPosition.y; }

// 方向をセット
void MapObject::setDirection(const Direction direction) { this->location.direction = direction; }

// オブジェクトIDをセット
void MapObject::setObjectId(int objectId) { this->objectId = objectId; }

// イベントIDをセット
void MapObject::setEventId(int eventId) { this->eventId = eventId; }

// イベントのtriggerをセット
void MapObject::setTrigger(Trigger trigger) { this->trigger = trigger; }

// 当たり判定の有無をセット
void MapObject::setHit(bool _isHit) { this->_isHit = _isHit; }

// 衝突判定用Rectをセット
void MapObject::setCollisionRect(const Rect& rect) { this->collisionRect = rect;}

// マップオブジェクトのリストをセット
void MapObject::setMapObjectList(MapObjectList* objectList) { this->objectList = objectList; }

// Spriteを設定
void MapObject::setSprite(Sprite* sprite)
{
    if(!sprite) return;
    
    this->sprite = sprite;
    
    this->addChild(sprite);
};

// SpriteFrameを追加
void MapObject::addSpriteFrame(SpriteFrame* spriteFrame)
{
    this->spriteFrames.pushBack(spriteFrame);
}

// ライトをセット
void MapObject::setLight(Light* light, AmbientLightLayer* ambientLightLayer, function<void()> callback)
{
    if(this->light) return;
    
    // ライトを追加
    this->light = light;
    this->addChild(light);
    light->setOpacity(0);
    light->runAction(Sequence::createWithTwoActions(FadeIn::create(0.5f), CallFunc::create(callback)));
    
    // 環境光レイヤーに光源として追加
    ambientLightLayer->addLightSource(light);
}

// ライトを消す
void MapObject::removeLight(function<void()> callback)
{
    if(!this->light) return;
    
    Light* light { this->light };
    this->light = nullptr;
    
    light->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create(callback), RemoveSelf::create(), nullptr));
}

// Locationを取得
Location MapObject::getLocation() const {return this->location;}

// オブジェクトIDを取得
int MapObject::getObjectId() const {return this->objectId;}

// イベントIDを取得
int MapObject::getEventId() const {return this->eventId;}

// triggerを取得
Trigger MapObject::getTrigger() const {return this->trigger;}

// 移動中かどうか
bool MapObject::isMoving() const {return this->_isMoving;}

// 現在キャラが向いている方向を取得
Direction MapObject::getDirection() const {return this->location.direction;}

// Spriteを取得
Sprite* MapObject::getSprite() const { return this->sprite;};

// SpriteFrameを取得
Vector<SpriteFrame*> MapObject::getSpriteFrames() const { return this->spriteFrames; };

// マップ上にある格子Rectを取得
vector<Rect> MapObject::getWorldGridCollisionRects()
{
    vector<Rect> gridCollisionRects {};
    
    for(MapObject* obj : this->objectList->getCollisionObjects({this}))
    {
        gridCollisionRects.push_back(obj->getGridRect());
    }
    
    return gridCollisionRects;
}

// 衝突判定用Rectを取得
Rect MapObject::getCollisionRect() const
{
    return Rect(this->getPosition().x + this->collisionRect.getMinX() - this->getContentSize().width / 2, this->getPosition().y + this->collisionRect.getMinY() - getContentSize().height / 2, this->collisionRect.size.width, this->collisionRect.size.height);
}

// 指定方向に移動した場合の衝突判定用Rectを取得
Rect MapObject::getCollisionRect(const Direction& direction) const
{
    vector<Direction> directions {direction};
    
    return this->getCollisionRect(directions);
}

// 指定2方向に移動した場合の衝突判定用Rectを取得
Rect MapObject::getCollisionRect(const vector<Direction>& directions) const
{
    Rect rect {this->getCollisionRect()};
    
    Point movementVec {Point::ZERO};
    
    // 二方向分の移動ベクトルを生成する
    for(int i { 0 }; i < directions.size(); i++)
    {
        if(i > 2) break;
        movementVec += MapUtils::getGridVector(directions[i]);
    }
    
    // あたり判定用Rectを縦横-2ピクセルした後に、x,y方向に1ピクセル足すことによって、関係ない範囲を巻き込まないようにしている（線分上、頂点上であっても判定がきいてしまうため）
    return Rect(rect.origin.x + 1 + movementVec.x, rect.origin.y + 1 + movementVec.y, rect.size.width - 2, rect.size.height - 2);
}

// 当たり判定の有無を取得
const bool MapObject::isHit() const {return this->_isHit;}

// 指定の方向に対して当たり判定があるか
const bool MapObject::isHit(const Direction& direction) const
{
    vector<Direction> directions {direction};
    
    return this->isHit(directions);
}

// 指定の２方向に対して当たり判定があるか
const bool MapObject::isHit(const vector<Direction>& directions) const
{
    if(!this->objectList) return false;
    
    // 自身以外の当たり判定を持つオブジェクトが、指定方向にあればtrueを返す
    for(MapObject* obj : this->objectList->getMapObjectsByGridRect(this->getGridRect(directions)))
    {
        if(obj->isHit() && obj != this) return true;
    }
    
    return false;
}

// 入力のあった方向から、移動可能方向のみを取り出して返す
vector<Direction> MapObject::createEnableDirections(const vector<Direction>& directions) const
{
    vector<Direction> enableDirs {};
    
    // 入力が複数か
    bool isInputMultiple {directions.size() >= 2};
    
    // 入力が２以上の時、斜め方向に当たり判定があるか確認
    bool isHitDiagnally { isInputMultiple ? this->isHit(directions) : false };
    
    // 当たり判定
    for(Direction direction : directions)
    {
        if((isInputMultiple && !isHitDiagnally) || (!isHitDiagnally && !this->isHit(direction)) || (isHitDiagnally && !this->isHit(direction) && enableDirs.empty()))
        {
            enableDirs.push_back(direction);
        }
    }
    
    return enableDirs;
}

// 方向から移動ベクトルを生成
Vec2 MapObject::createMoveVec(const vector<Direction>& directions, const bool check) const
{
    // 当たり判定チェックが必要な場合は、メソッドから、そうでない場合は素の方向から移動ベクトルを生成する
    vector<Direction> dirs {check ? this->createEnableDirections(directions) : directions};
    
    Vec2 movement {Vec2::ZERO};
    
    for(Direction direction : dirs)
    {
        movement += MapUtils::getGridVector(direction);
    }
    
    return movement;
}

// 入力方向に対して動くことが可能かどうか
bool MapObject::canMove(const vector<Direction>& directions) const {return !this->createEnableDirections(directions).empty();}

// 当たり判定、地形チェックをせずにして方向に移動させる
void MapObject::move(const vector<Direction>& enableDirections, function<void()> onMoved, const float ratio)
{
    // 方向からベクトル生成
    Vec2 movement { this->createMoveVec(enableDirections, false) };
    
    // マス座標を変更
    this->setGridPosition(this->getGridPosition() + Vec2(movement.x, -movement.y) / GRID);
    
    // 移動開始
    this->_isMoving = true;
    this->runAction(Sequence::create(MoveBy::create(DURATION_MOVE_ONE_GRID / ratio, movement), CallFunc::create([this]
    {
        this->_isMoving = false;
        if(this->onMoved) this->onMoved(this);
    }), CallFunc::create(onMoved), nullptr));
}

// 方向指定移動メソッド
bool MapObject::moveBy(const Direction& direction, function<void()> onMoved, const float ratio) {return this->moveBy({direction}, onMoved, ratio);}

// 方向指定移動メソッド
bool MapObject::moveBy(const vector<Direction>& directions, function<void()> onMoved, const float ratio)
{
    if(directions.empty()) return false;
    
    // 移動可能な方向を生成
    vector<Direction> dirs { this->createEnableDirections(directions) };
    
    // 移動可能な方向がなければ失敗としてリターン
    if(dirs.empty()) return false;

    // 地形オブジェクトに移動をイベント送信
    this->getTerrain(dirs)->onWillMove(this, dirs, onMoved, ratio);
    
    return true;
}

// 方向、マス数指定移動用メソッド
void MapObject::moveBy(const Direction& direction, const int gridNum, function<void(bool)> callback, const float ratio) {this->moveBy({direction}, gridNum, callback, ratio);}

// 複数方向、マス数指定移動用メソッド
void MapObject::moveBy(const vector<Direction>& directions, const int gridNum, function<void(bool)> callback, const float ratio)
{
    if(directions.empty() || this->isMoving()) return;
    
    deque<vector<Direction>> directionsQueue {};
    
    // 方向をキューに登録
    for(int i { 0 }; i < gridNum; i++)
    {
        directionsQueue.push_back(directions);
    }
    
    // キューに登録した動きを実行
    this->moveByQueue(directionsQueue, callback, ratio);
}

void MapObject::moveByQueue(deque<Direction> directionQueue, function<void(bool)> callback, const float ratio)
{
    if(directionQueue.empty())
    {
        if(callback) callback(true);
        
        return;
    }
    
    deque<vector<Direction>> directionsQueue {};
    
    for(Direction direction : directionQueue)
    {
        directionsQueue.push_back(vector<Direction>({direction}));
    }
    
    this->moveByQueue(directionsQueue, callback, ratio);
}

// キューから動かす
void MapObject::moveByQueue(deque<vector<Direction>> directionsQueue, function<void(bool)> callback, const float ratio)
{
    // 初回呼び出し以外は空で渡されるため、空でない時は新たに格納する
    if(!directionsQueue.empty()) this->directionsQueue = directionsQueue;
    
    // キューが空になったら成功としてコールバックを呼び出し
    if(this->directionsQueue.empty())
    {
        callback(true);
        
        return;
    }
    
    // キューの先頭を実行。失敗時にはコールバックを失敗として実行
    vector<Direction> directions { this->directionsQueue.front() };
    this->directionsQueue.pop_front();
    
    // 移動開始。失敗時はコールバックを失敗として呼び出し
    if(!this->moveBy(directions, [this, callback, ratio]{this->moveByQueue(deque<vector<Direction>>({}), callback, ratio);}, ratio)) callback(false);
}

// 移動用方向キューをクリア
void MapObject::clearDirectionsQueue()
{
    mutex mtx;
    mtx.lock();
    this->directionsQueue.clear();
    mtx.unlock();
}

// 指定方向にある地形オブジェクトを取得
TerrainObject* MapObject::getTerrain(const vector<Direction>& directions)
{
    if(!this->objectList) return nullptr;
    
    return this->objectList->getTerrainByGridRect(this->getGridRect(directions));
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

// 指定方法をマップ上での方向に変換
Direction MapObject::convertToWorldDir(const Direction direction)
{
    switch (this->getDirection()) {
        case Direction::FRONT:
            return MapUtils::oppositeDirection(direction);
            
        case Direction::BACK:
            return direction;
        
        case Direction::RIGHT:
            if(direction == Direction::BACK) return Direction::LEFT;
            if(direction == Direction::FRONT) return Direction::RIGHT;
            if(direction == Direction::RIGHT) return Direction::FRONT;
            if(direction == Direction::LEFT) return Direction::BACK;
            
        case Direction::LEFT:
            if(direction == Direction::BACK) return Direction::RIGHT;
            if(direction == Direction::FRONT) return Direction::LEFT;
            if(direction == Direction::RIGHT) return Direction::BACK;
            if(direction == Direction::LEFT) return Direction::FRONT;
            
        default:
            return Direction::SIZE;
    }
}

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
}
