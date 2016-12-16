//
//  TerrainStateCache.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/03.
//
//

#ifndef TerrainStateCache_h
#define TerrainStateCache_h

#include "define.h"

class TerrainState;

class TerrainStateCache : public Ref
{
// 定数
public:
    enum struct StateType
    {
        NORMAL,
        SLIP,
        SWIM,
        LADDER,
    };

// クラスメソッド
public:
    CREATE_FUNC(TerrainStateCache)
    
// インスタンス変数
private:
    map<StateType, TerrainState*> _typeToState;
    
// インスタンスメソッド
private:
    TerrainStateCache();
    virtual ~TerrainStateCache();
    virtual bool init();
    
public:
    TerrainState* getState(StateType type);
};

#endif /* TerrainStateCache_h */
