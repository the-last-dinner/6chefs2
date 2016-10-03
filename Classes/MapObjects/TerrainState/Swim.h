//
//  Swim.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/03.
//
//

#ifndef Swim_h
#define Swim_h

#include "MapObjects/TerrainState/TerrainState.h"

NS_MO_T_STATE_BEGIN

class Swim : public TerrainState
{
// クラスメソッド
public:
    CREATE_FUNC(Swim)
// インスタンスメソッド
private:
    Swim();
    virtual ~Swim();
    virtual bool init() override;
    
// インターフェース
public:
    void move(MapObject* target, const vector<Direction>& directions, function<void()> cb, float speed) override;
    void stamp(Character* target, const Direction& direction, float speed) override;
};

NS_MO_T_STATE_END

#endif /* Swim_h */
