//
//  Torch.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/19.
//
//

#include "Effects/LightSources/Torch.h"

#include "Effects/AmbientLightLayer.h"

bool Torch::init()
{
    this->createInnerLight();
    this->createOuterLight();
    
    return true;
}

void Torch::update()
{
    
}

void Torch::remove()
{
    LightSource::remove();
    CC_SAFE_RELEASE_NULL(_innerLight);
    CC_SAFE_RELEASE_NULL(_outerLight);
    this->removeFromParent();
}

void Torch::createInnerLight()
{
    Light::Information innerInfo {Light::TYPE_TO_INFO.at(Light::Type::TORCH)};
    _innerLight = Light::create(innerInfo);
    _innerLight->setOpacity(0);
    _innerLight->setBlendFunc(BlendFunc{ GL_SRC_ALPHA, GL_ONE });
    
    CC_SAFE_RETAIN(_innerLight);
}

void Torch::createOuterLight()
{
    if (!_innerLight) return;
    
    Light::Information innerInfo {_innerLight->getInformation()};
    Color3B color { Color3B(innerInfo.color.r * 1.3f, innerInfo.color.g * 1.3f, innerInfo.color.b * 1.3f)};
    float radius {innerInfo.radius * 7.0f};
    
    Light::Information outerInfo {Light::Information(color, radius, innerInfo.fileName)};
    _outerLight = Light::create(outerInfo);
    
    _outerLight->setPosition(_innerLight->convertToWorldSpace(_innerLight->getPosition()));
    _outerLight->setOpacity(0);
    _outerLight->setBlendFunc(BlendFunc{GL_SRC_COLOR, GL_ONE});
    //_innerLight->ambientLight->addChild(_outerLight);
    
    CC_SAFE_RETAIN(_outerLight);
}
