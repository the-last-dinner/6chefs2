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
    CREATE_FUNC(LightSource);
    LightSource(){FUNCLOG};
    ~LightSource(){FUNCLOG};
    
protected:
    Light* _innerLight {nullptr};
    Light* _outerLight {nullptr};
    
public:
    virtual bool init();
    virtual void update(){};
    virtual void remove();
    void start(function<void()> callback);
    Light* getInnerLight();
    Light* getOuterLight();
protected:
    virtual void createInnerLight(){};
    virtual void createOuterLight(){};
};

#endif /* LightSource_hpp */
