//
//  AttackBox.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/19.
//
//

#ifndef AttackBox_h
#define AttackBox_h

#include "MapObjects/DetectionBox/DetectionBox.h"

class AttackBox : public DetectionBox
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(AttackBox, MapObject*, Node*)
    CREATE_FUNC_WITH_TWO_PARAM(AttackBox, MapObject*, const Rect&)
    
// インスタンスメソッド
private:
    AttackBox();
    virtual ~AttackBox();
    virtual bool init(MapObject* parent, Node* origin) override;
    virtual bool init(MapObject* parent, const Rect& originRect) override;
};

#endif /* AttackBox_h */
