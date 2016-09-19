//
//  HitBox.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/19.
//
//

#ifndef HitBox_h
#define HitBox_h

#include "MapObjects/DetectionBox/DetectionBox.h"

class HitBox : public DetectionBox
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(HitBox, MapObject*, Node*)
    CREATE_FUNC_WITH_TWO_PARAM(HitBox, MapObject*, const Rect&)
    
// インスタンスメソッド
private:
    HitBox();
    virtual ~HitBox();
    virtual bool init(MapObject* parent, Node* origin) override;
    virtual bool init(MapObject* parent, const Rect& originRect) override;
};

#endif /* HitBox_h */
