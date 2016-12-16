//
//  Ladder.h
//  6chefs2
//
//  Created by S.inami on 2016/12/16.
//
//

#ifndef Ladder_h
#define Ladder_h

#include "MapObjects/TerrainState/TerrainState.h"

NS_MO_T_STATE_BEGIN

class Ladder : public TerrainState
{
    // クラスメソッド
public:
    CREATE_FUNC(Ladder)
    
    // インスタンスメソッド
private:
    Ladder();
    virtual ~Ladder();
    virtual bool init() override;
    
    // インターフェース
public:
    void move(MapObject* target, const vector<Direction>& directions, function<void()> cb, float speed) override;
    void stamp(Character* target, const Direction& direction, float speed) override;
};

NS_MO_T_STATE_END

#endif /* Ladder_h */
