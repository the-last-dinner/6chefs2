//
//  StartUpSceneData.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/13.
//
//

#ifndef StartUpSceneData_h
#define StartUpSceneData_h

#include "Datas/Scene/SceneData.h"

class StartUpSceneData : public SceneData
{
    // クラスメソッド
public:
    CREATE_FUNC(StartUpSceneData)
    
    // インスタンスメソッド
private:
    StartUpSceneData() {FUNCLOG};
    ~StartUpSceneData() {FUNCLOG};
    bool init();
};
#endif /* StartUpSceneData_h */
