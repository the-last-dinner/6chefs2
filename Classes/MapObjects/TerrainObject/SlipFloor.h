//
//  SlipFloor.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#ifndef SlipFloor_h
#define SlipFloor_h

#include "MapObjects/TerrainObject/TerrainObject.h"

class SlipFloor : public TerrainObject
{
// クラスメソッド
public:
    CREATE_FUNC(SlipFloor)
    
// インスタンスメソッド
private:
    SlipFloor();
    ~SlipFloor();
    bool init() override;
    
// インターフェース
public:
    virtual TerrainState* getTerrainState(TerrainStateCache* cache) const override;
};

#endif /* SlipFloor_h */
