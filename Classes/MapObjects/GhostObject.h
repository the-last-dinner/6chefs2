//
//  GhostObject.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#ifndef GhostObject_h
#define GhostObject_h

#include "MapObjects/MapObject.h"

// 当たり判定をすり抜けるオブジェクト
class GhostObject : public MapObject
{
// クラスメソッド
public:
    CREATE_FUNC(GhostObject)
    
// インスタンスメソッド
private:
    GhostObject();
    ~GhostObject();
    bool init() override;
    
public:
    virtual vector<Rect> getWorldGridCollisionRects() override;
    virtual const bool isHit(const vector<Direction>& directions) const override;
};

#endif /* GhostObject_h */
