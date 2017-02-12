//
//  OpeningSceneData.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/13.
//
//

#ifndef OpeningSceneData_h
#define OpeningSceneData_h

#include "Datas/Scene/SceneData.h"

class OpeningSceneData : public SceneData
{
    // クラスメソッド
public:
    CREATE_FUNC(OpeningSceneData)
    
    // インスタンスメソッド
private:
    OpeningSceneData() {FUNCLOG};
    ~OpeningSceneData() {FUNCLOG};
    bool init();
};

#endif /* OpeningSceneData_hpp */
