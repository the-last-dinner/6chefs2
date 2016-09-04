//
//  TerrainObject.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#ifndef TerrainObject_h
#define TerrainObject_h

#include "MapObjects/MapObject.h"

class Character;
class TerrainState;
class TerrainStateCache;

class TerrainObject : public MapObject
{    
// インスタンスメソッド
protected:
    virtual bool init() override;
    TerrainObject();
    virtual ~TerrainObject();
    
// インターフェース
public:
    virtual TerrainState* getTerrainState(TerrainStateCache* cache) const = 0;
};

#endif /* TerrainObject_h */
