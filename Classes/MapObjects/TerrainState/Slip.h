//
//  Slip.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/03.
//
//

#ifndef Slip_h
#define Slip_h

#include "MapObjects/TerrainState/TerrainState.h"

NS_MO_T_STATE_BEGIN

class Slip : public TerrainState
{
// クラスメソッド
public:
    CREATE_FUNC(Slip)

// インスタンスメソッド
private:
    Slip();
    virtual ~Slip();
    virtual bool init() override;
    
// インターフェース
public:
    void move(MapObject* target, const vector<Direction>& directions, function<void()> cb, float speed) override;
    void stamp(Character* target, const Direction& direction, float speed) override;
};

NS_MO_T_STATE_END

#endif /* Slip_h */
