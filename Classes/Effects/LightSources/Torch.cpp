//
//  Torch.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/19.
//
//

#include "Effects/LightSources/Torch.h"

#include "Effects/AmbientLightLayer.h"

const Color3B Torch::COLOR {Color3B(195, 110, 60)};
const string Torch::IMAGE {"light.png"};

bool Torch::init(Light::Information& info)
{
    this->createInnerLight(info);
    this->createOuterLight();
    this->readyLight();
    
    return true;
}

void Torch::update()
{
    
}

void Torch::createInnerLight(Light::Information& info)
{
    info.image = Torch::IMAGE;
    info.type = Light::Type::TORCH;
    _innerLight = Light::create(info);
}

void Torch::createOuterLight()
{
    if (!_innerLight) return;
    
    Light::Information innerInfo {_innerLight->getInformation()};
    Color3B color { Color3B(innerInfo.color.r * 1.3f, innerInfo.color.g * 1.3f, innerInfo.color.b * 1.3f)};
    float radius {innerInfo.radius * 7.0f};
    
    Light::Information outerInfo {Light::Information(color, radius, innerInfo.image)};
    _outerLight = Light::create(outerInfo);
}
