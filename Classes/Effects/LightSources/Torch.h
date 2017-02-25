//
//  Torch.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/19.
//
//

#ifndef Torch_h
#define Torch_h

#include "Effects/LightSources/LightSource.h"

class Light;

class Torch : public LightSource
{
public:
    static const Color3B COLOR;
    static const string IMAGE;
    
public:
    CREATE_FUNC_WITH_PARAM(Torch, Light::Information&);
    Torch(){FUNCLOG};
    ~Torch(){FUNCLOG};
    
public:
    bool init(Light::Information& info) override;
    void update() override;
protected:
    void createInnerLight(Light::Information& info) override;
    void createOuterLight() override;
};

#endif /* Torch_hpp */
