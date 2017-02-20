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

class Torch : public LightSource
{
public:
    CREATE_FUNC(Torch);
    Torch(){FUNCLOG};
    ~Torch(){FUNCLOG};
    
public:
    bool init() override;
    void update() override;
    void remove() override;
protected:
    void createInnerLight() override;
    void createOuterLight() override;
};

#endif /* Torch_hpp */
