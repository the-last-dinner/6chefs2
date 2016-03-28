//
//  Light.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/13.
//
//

#include "Effects/Light.h"

// 定数
const Color3B Light::TORCH_COLOR {Color3B(195, 110, 60)};

// コンストラクタ
Light::Light() { FUNCLOG }

// デストラクタ
Light::~Light() { FUNCLOG }

// 初期化
bool Light::init(const Information& info)
{
	if(!Node::init()) return false;
    
    this->info = info;
	
	Sprite* light {Sprite::createWithSpriteFrameName("light.png")};
	light->setColor(info.color);
	float scale {(info.radius * 2) / light->getContentSize().width};
	light->setScale(scale);
	this->setContentSize(light->getContentSize() * scale);
	this->addChild(light);
    this->sprite = light;
    
	this->setCascadeOpacityEnabled(true);
    this->setCascadeColorEnabled(true);
    
    if(info.type == Light::Type::TORCH) this->runAction(Sequence::create(Repeat::create(Sequence::create(FadeTo::create(0.5f, 230), FadeTo::create(0.5f, 255), nullptr), -1), nullptr));
	
	return true;
}

Light::Information Light::getInformation()
{
    return this->info;
}

void Light::setBlendFunc(const BlendFunc& blendFunc)
{
    this->sprite->setBlendFunc(blendFunc);
}
