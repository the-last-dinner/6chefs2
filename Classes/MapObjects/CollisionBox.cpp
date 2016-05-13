//
//  CollisionBox.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/08.
//
//

#include "MapObjects/CollisionBox.h"

# pragma mark private

// コンストラクタ
CollisionBox::CollisionBox() { FUNCLOG };

// デストラクタ
CollisionBox::~CollisionBox() { FUNCLOG };

// 初期化
bool CollisionBox::init(Node* origin)
{
    if(!Node::init()) return false;
    
    this->origin = origin;
    this->setRect(origin->getBoundingBox());
    
    return true;
}

// 初期化
bool CollisionBox::init(const Rect& rect)
{
    if(!Node::init()) return false;
    
    this->setRect(rect);
    
    return true;
}

// Rectを設定
void CollisionBox::setRect(const Rect& rect)
{
    this->setContentSize(rect.size);
}

# pragma mark -
# pragma mark public

// マップに配置された時
void CollisionBox::onEnterMap()
{
    if(this->origin) this->scheduleUpdate();
}

void CollisionBox::update(float delta)
{
    this->setPosition(this->origin->getPosition());
    this->setRect(this->origin->getBoundingBox());
}
