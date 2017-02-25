//
//  LightSource.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/19.
//
//

#ifndef LightSource_h
#define LightSource_h

#include "Common.h"

#include "Effects/Light.h"

class LightSource : public Node
{
public:
    CREATE_FUNC_WITH_PARAM(LightSource, Light::Information&);
    LightSource(){FUNCLOG};
    ~LightSource(){FUNCLOG};
    
protected:
    Light* _innerLight {nullptr};
    Light* _outerLight {nullptr};
    
public:
    virtual bool init(Light::Information& info);
    virtual void update(){};
    virtual void remove();
    void lightUp(function<void()> callback);
    void readyLight();
    Light* getInnerLight();
    Light* getOuterLight();
protected:
    virtual void createInnerLight(Light::Information& info){};
    virtual void createOuterLight(){};
};

#endif /* LightSource_hpp */
