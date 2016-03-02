//
//  EndingSceneData.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/03.
//
//

#ifndef EndingSceneData_h
#define EndingSceneData_h

#include "Datas/Scene/SceneData.h"

class EndingSceneData : public SceneData
{
// クラスメソッド
public:
    CREATE_FUNC(EndingSceneData)
    
// インスタンスメソッド
private:
    EndingSceneData();
    ~EndingSceneData();
    bool init();
};

#endif /* EndingSceneData_h */
