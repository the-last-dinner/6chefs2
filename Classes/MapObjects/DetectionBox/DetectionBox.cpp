//
//  DetectionBox.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/06.
//
//

#include "MapObjects/DetectionBox/DetectionBox.h"

#include "MapObjects/MapObject.h"

// コンストラクタ
DetectionBox::DetectionBox() {}

// デストラクタ
DetectionBox::~DetectionBox() {}

// 初期化
bool DetectionBox::init(MapObject* parent, Node* origin)
{
    if (!Node::init()) return false;
    if (!parent) return false;
    
    _parent = parent;
    _origin = origin;
    
    this->setOriginInfo(origin);
    
    return true;
}

// 初期化
bool DetectionBox::init(MapObject* parent, const Rect& rect)
{
    if (!Node::init()) return false;
    if (!parent) return false;
    
    _parent = parent;
    
    this->setOriginInfo(rect);
    
    return true;
}

void DetectionBox::setOriginInfo(Node* origin)
{
    if (!origin) return;
    
    this->setOriginInfo(origin->getBoundingBox());
}

void DetectionBox::setOriginInfo(const Rect& originRect)
{
    this->setPosition(originRect.getMinX(), originRect.getMinY());
    this->setContentSize(originRect.size);
}

void DetectionBox::startDetection()
{
    if (this->isScheduled(CC_SCHEDULE_SELECTOR(DetectionBox::update))) return;
    this->scheduleUpdate();
}

void DetectionBox::update(float delta)
{
    if (!_origin) return;
    
    this->setOriginInfo(_origin);
}

#pragma mark -
#pragma mark GridRect

Rect DetectionBox::getGridRect(const vector<Direction>& directions) const
{
    if (!_parent) return Rect::ZERO;
    
    Vec2 gridVec { Direction::getGridVec2(directions) };
    Point parentGridPosition { _parent->getGridPosition() };
    Rect rect { this->getBoundingBox() };
    
    float x { parentGridPosition.x + gridVec.x + rect.getMinX() / GRID };
    float y { parentGridPosition.y + gridVec.y + rect.getMinY() / GRID };
    float width { rect.size.width / GRID };
    float height { rect.size.height / GRID };
    
    return Rect(x, y, width, height);
}

bool DetectionBox::intersectsGrid(DetectionBox* other, const vector<Direction>& directions) const
{
    if (other == this) return false;
    if (!_parent->isHit(other->_parent)) return false;
    
    return this->intersectsGrid(other->getGridRect(), directions);
}

bool DetectionBox::intersectsGrid(const Rect& gridRect, const vector<Direction>& directions) const
{
    return MapUtils::intersectsGridRect(this->getGridRect(directions), gridRect);
}

#pragma mark -
#pragma mark Rect

Rect DetectionBox::getRect() const
{
    Rect rect { this->getBoundingBox() };
    Rect parentRect { _parent->getBoundingBox() };
    
    float x { parentRect.getMinX() + rect.getMinX() };
    float y { parentRect.getMinY() + rect.getMinY() };
    float width { rect.size.width };
    float height { rect.size.height };
    
    return Rect(x, y, width, height);
}

Rect DetectionBox::getRect(const Direction& direction) const
{
    return this->getRect(vector<Direction>{ direction });
}

Rect DetectionBox::getRect(const vector<Direction>& directions) const
{
    Rect rect { this->getRect() };
    Vec2 vec2 { Direction::getVec2(directions) };
    
    // あたり判定用Rectを縦横-2ピクセルした後に、x,y方向に1ピクセル足すことによって、関係ない範囲を巻き込まないようにしている（線分上、頂点上であっても判定がきいてしまうため）
    return Rect(rect.origin.x + 1 + vec2.x, rect.origin.y + 1 + vec2.y, rect.size.width - 2, rect.size.height - 2);
}

bool DetectionBox::isBetween(const MapObject* obj1, const MapObject* obj2) const
{
    if (!obj1) return false;
    if (!obj2) return false;
    if (obj1 == _parent) return false;
    if (obj2 == _parent) return false;
    
    return MapUtils::isSegmentIntersectWithRect(obj1->getPosition(), obj2->getPosition(), this->getRect());
}

bool DetectionBox::intersects(const DetectionBox* other) const
{
    if(_parent == other->_parent) return false;
    
    return this->getRect().intersectsRect(other->getRect());
}
