//
//  PlainArea.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#ifndef PlainArea_h
#define PlainArea_h

#include "MapObjects/TerrainObject/TerrainObject.h"

class PlainArea : public TerrainObject
{
// クラスメソッド
public:
    CREATE_FUNC(PlainArea)
    
// インスタンスメソッド
private:
    PlainArea();
    ~PlainArea();
    bool init() override;
    
// インターフェース
public:
    virtual TerrainState* getTerrainState(TerrainStateCache* cache) const override;
};

#endif /* PlainArea_h */
