//
//  SlipFloor.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/01.
//
//

#include "MapObjects/TerrainObject/SlipFloor.h"

// コンストラクタ
SlipFloor::SlipFloor() {FUNCLOG};

// デストラクタ
SlipFloor::~SlipFloor() {FUNCLOG};

// 初期化
bool SlipFloor::init()
{
    return true;
}

// オブジェクトが動こうとした時
void SlipFloor::onWillMove(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio)
{
    // コールバックを変更
    target->move(directions, [target, directions, onMoved, ratio]
    {
        if(!target->moveBy(directions, onMoved, ratio) && onMoved)  onMoved();
    }, ratio);
}