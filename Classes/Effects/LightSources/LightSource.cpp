//
//  LightSource.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/19.
//
//

#include "Effects/LightSources/LightSource.h"

bool LightSource::init()
{
    return true;
}

void LightSource::start(function<void()> callback)
{
    float duration {0.5f};
    
    this->runAction(
        Sequence::createWithTwoActions(
            CallFunc::create([this, duration](){
                _innerLight->runAction(FadeIn::create(duration));
                _outerLight->runAction(FadeIn::create(duration));
            }),
            CallFunc::create(callback)
        )
    );
}

void LightSource::remove()
{
    
}

Light* LightSource::getInnerLight()
{
    return _innerLight;
}

Light* LightSource::getOuterLight()
{
    return _outerLight;
}
