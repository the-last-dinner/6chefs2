//
//  TerrainStateCache.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/03.
//
//

#include "MapObjects/TerrainState/TerrainStateCache.h"

#include "MapObjects/TerrainState/Normal.h"
#include "MapObjects/TerrainState/Slip.h"
#include "MapObjects/TerrainState/Swim.h"
#include "MapObjects/TerrainState/Ladder.h"

USING_NS_MO_T_STATE

// コンストラクタ
TerrainStateCache::TerrainStateCache() { FUNCLOG }

// デストラクタ
TerrainStateCache::~TerrainStateCache()
{
    FUNCLOG
    
    for(pair<StateType, TerrainState*> p : _typeToState)
    {
        CC_SAFE_RELEASE(p.second);
    }
}

// 初期化
bool TerrainStateCache::init()
{
    TerrainState* normal { Normal::create() };
    TerrainState* slip { Slip::create() };
    TerrainState* swim { Swim::create() };
    TerrainState* ladder { Ladder::create() };
    
    CC_SAFE_RETAIN(normal);
    CC_SAFE_RETAIN(slip);
    CC_SAFE_RETAIN(swim);
    CC_SAFE_RETAIN(ladder);
    
    map<StateType, TerrainState*> typeToState {
        { StateType::NORMAL, normal },
        { StateType::SLIP,   slip   },
        { StateType::SWIM,   swim   },
        { StateType::LADDER, ladder },
    };
    
    _typeToState = typeToState;
    
    return true;
}

TerrainState* TerrainStateCache::getState(StateType type)
{
    if(_typeToState.count(type) == 0) return nullptr;
    
    return _typeToState.at(type);
}
