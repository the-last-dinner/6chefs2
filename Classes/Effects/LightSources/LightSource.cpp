//
//  LightSource.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/19.
//
//

#include "Effects/LightSources/LightSource.h"

bool LightSource::init(Light::Information& info)
{
    return true;
}

void LightSource::readyLight()
{
    if (!_innerLight) return;
    
    _innerLight->setOpacity(0);
    _innerLight->setBlendFunc(BlendFunc{ GL_SRC_ALPHA, GL_ONE });
    this->addChild(_innerLight);
    
    if (!_outerLight) return;
    
    _outerLight->setOpacity(0);
    _outerLight->setBlendFunc(BlendFunc{GL_SRC_COLOR, GL_ONE});
    _outerLight->setPosition(_innerLight->getAnchorPoint());
    _innerLight->addChild(_outerLight);
}

void LightSource::lightUp(function<void()> callback)
{
    float duration {0.5f};
    
    this->runAction(
        Sequence::createWithTwoActions(
            FadeIn::create(duration),
            CallFunc::create(callback)
        )
    );
}

void LightSource::remove()
{
    this->removeFromParent();
}


Light* LightSource::getInnerLight()
{
    return _innerLight;
}

Light* LightSource::getOuterLight()
{
    return _outerLight;
}
