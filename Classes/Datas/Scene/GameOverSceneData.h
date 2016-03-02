//
//  GameOverSceneData.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#ifndef GameOverSceneData_h
#define GameOverSceneData_h

#include "Datas/Scene/SceneData.h"

class GameOverSceneData : public SceneData
{
// クラスメソッド
public:
    CREATE_FUNC(GameOverSceneData)
    
// インスタンスメソッド
private:
    GameOverSceneData();
    ~GameOverSceneData();
    bool init();
};

#endif /* GameOverSceneData_h */
