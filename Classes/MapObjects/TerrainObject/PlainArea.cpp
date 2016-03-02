//
//  PlainArea.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#include "MapObjects/TerrainObject/PlainArea.h"

// コンストラクタ
PlainArea::PlainArea() {FUNCLOG};

// デストラクタ
PlainArea::~PlainArea() {FUNCLOG};

// 初期化
bool PlainArea::init()
{
    return true;
}

// オブジェクトが動こうとした時
void PlainArea::onWillMove(MapObject* target, const vector<Direction>& directions, function<void()> onMoved, const float ratio)
{
    // 移動
    target->move(directions, onMoved, ratio);
}