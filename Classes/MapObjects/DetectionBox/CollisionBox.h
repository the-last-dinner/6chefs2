//
//  CollisionBox.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/11.
//
//

#ifndef CollisionBox_h
#define CollisionBox_h

#include "MapObjects/DetectionBox/DetectionBox.h"

class MapObject;

class CollisionBox : public DetectionBox
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(CollisionBox, MapObject*, Node*)
    CREATE_FUNC_WITH_TWO_PARAM(CollisionBox, MapObject*, const Rect&)

// インスタンスメソッド
private:
    CollisionBox();
    virtual ~CollisionBox();
    virtual bool init(MapObject* parent, Node* origin) override;
    virtual bool init(MapObject* parent, const Rect& originRect) override;
    
// Debug
private:
    virtual Color4F getDebugMaskColor() const override;
};

#endif /* CollisionBox_h */
