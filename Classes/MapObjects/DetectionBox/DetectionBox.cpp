//
//  DetectionBox.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/06.
//
//

#include "MapObjects/DetectionBox/DetectionBox.h"

#include "MapObjects/MapObject.h"

#include "Managers/ConfigDataManager.h"
#include "Models/ConfigData/DebugConfigData.h"

// コンストラクタ
DetectionBox::DetectionBox() {}

// デストラクタ
DetectionBox::~DetectionBox() {}

// 初期化
bool DetectionBox::init(MapObject* parent, Node* origin)
{
    if (!Node::init()) return false;
    if (!parent) return false;
    if (!origin) return false;
    
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

// 親を取得
MapObject* DetectionBox::getMOParent() const
{
    return _parent;
}

// 有効か
bool DetectionBox::isEnabled() const
{
    if (!_origin) return true;
    return _origin->isVisible();
}

void DetectionBox::setOriginInfo(const Rect& originRect)
{
    this->setPosition(originRect.getMinX(), originRect.getMinY());
    this->setContentSize(originRect.size);
}

void DetectionBox::update(float delta)
{
    if (_origin) this->setOriginInfo(_origin);
    
    if (ConfigDataManager::getInstance()->getDebugConfigData()->getBoolValue(DebugConfigData::DEBUG_MASK)) {
        this->drawDebugMask();
    }
}

#pragma mark -
#pragma mark GridRect

Rect DetectionBox::getGridRect(const vector<Direction>& directions) const
{
    if (!_parent) return Rect::ZERO;
    
    Vec2 gridVec { Direction::getGridVec2(directions) };
    Point parentGridPosition { _parent->getGridPosition() };
    Rect gridRect { this->getGridRect(parentGridPosition) };
    
    gridRect.origin.x += gridVec.x;
    gridRect.origin.y += gridVec.y;
    
    return gridRect;
}

Rect DetectionBox::getGridRect(const Point& parentGridPosition) const
{
    Rect rect { this->getBoundingBox() };
    
    float x { parentGridPosition.x + rect.getMinX() / GRID };
    float y { parentGridPosition.y + rect.getMinY() / GRID };
    float width { rect.size.width / GRID };
    float height { rect.size.height / GRID };
    
    return Rect(x, y, width, height);
}

bool DetectionBox::intersectsGrid(DetectionBox* other, const vector<Direction>& directions) const
{
    if (other == this) return false;
    
    return MapUtils::intersectsGridRect(this->getGridRect(directions), other->getGridRect());
}

bool DetectionBox::intersectsGrid(DetectionBox* other, const Point& gridPosition) const
{
    if (other == this) return false;
    
    return MapUtils::intersectsGridRect(this->getGridRect(gridPosition), other->getGridRect());
}

bool DetectionBox::intersectsGridForPath(DetectionBox* other, const Point& gridPosition) const
{
    if (other == this) return false;
    if (!_parent->isHit(other->_parent)) return false;
    
    return MapUtils::intersectsGridRect(this->getGridRect(gridPosition), other->getGridRect());
}

#pragma mark -
#pragma mark Rect

Rect DetectionBox::getRect() const
{
    Rect rect { this->getBoundingBox() };
    
    float x { _parent->getPosition().x + rect.getMinX() - _parent->getContentSize().width / 2 };
    float y { _parent->getPosition().y + rect.getMinY() - _parent->getContentSize().height / 2 };
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

bool DetectionBox::isBetween(const DetectionBox* other1, const DetectionBox* other2) const
{
    if (!other1) return false;
    if (!other2) return false;
    if (other1->_parent == _parent) return false;
    if (other2->_parent == _parent) return false;
    
    Rect other1Rect { other1->getRect() };
    Rect other2Rect { other2->getRect() };
    
    Point other1Pos { other1Rect.getMidX(), other1Rect.getMidY() };
    Point other2Pos { other2Rect.getMidX(), other2Rect.getMidY() };
    
    return MapUtils::isSegmentIntersectWithRect(other1Pos, other2Pos, this->getRect());
}

bool DetectionBox::intersects(const DetectionBox* other) const
{
    if(_parent == other->_parent) return false;
    
    return this->getRect().intersectsRect(other->getRect());
}

#pragma mark -
#pragma mark Debug

void DetectionBox::drawDebugMask()
{
    Node* debugMask { this->getChildByName("debug_mask") };
    if (debugMask) {
        this->removeChild(debugMask);
    }
    
    if (!this->isEnabled()) return;
    
    Rect rect { this->getBoundingBox() };
    
    Point vertices[]
    {
        Point::ZERO,
        Point(0, rect.size.height),
        rect.size,
        Point(rect.size.width, 0),
        Point::ZERO
    };
    Color4F lineColor { this->getDebugMaskColor() };
    DrawNode* draw { DrawNode::create() };
    draw->drawPolygon(vertices, 5, Color4F(0,0,0,0), 1, lineColor);
    draw->setPosition(rect.size.width / -2, rect.size.height / -2);
    draw->setGlobalZOrder(Priority::DEBUG_MASK);
    draw->setName("debug_mask");
    this->addChild(draw);
}

Color4F DetectionBox::getDebugMaskColor() const
{
    return Color4F::WHITE;
}
