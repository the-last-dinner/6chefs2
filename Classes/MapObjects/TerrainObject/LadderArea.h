//
//  LadderArea.h
//  6chefs2
//
//  Created by S.inami on 2016/12/16.
//
//

#ifndef LadderArea_h
#define LadderArea_h

#include "MapObjects/TerrainObject/TerrainObject.h"

class LadderArea : public TerrainObject
{
    // クラスメソッド
public:
    CREATE_FUNC(LadderArea)
    
    // インスタンスメソッド
private:
    LadderArea();
    ~LadderArea();
    bool init() override;
    
    // インターフェース
public:
    virtual TerrainState* getTerrainState(TerrainStateCache* cache) const override;
};

#endif /* LadderArea_h */
