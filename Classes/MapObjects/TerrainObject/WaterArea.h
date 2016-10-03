//
//  WaterArea.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#ifndef Water_h
#define Water_h

#include "MapObjects/TerrainObject/TerrainObject.h"

class WaterArea : public TerrainObject
{
// クラスメソッド
public:
    CREATE_FUNC(WaterArea)

// インスタンスメソッド
private:
    WaterArea();
    ~WaterArea();
    bool init() override;

// インターフェース
public:
    virtual TerrainState* getTerrainState(TerrainStateCache* cache) const override;
};

#endif /* Water_h */
