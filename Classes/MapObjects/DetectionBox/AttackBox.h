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
    CREATE_FUNC_WITH_THREE_PARAM(AttackBox, MapObject*, Node*, function<void(MapObject*)>)
    CREATE_FUNC_WITH_THREE_PARAM(AttackBox, MapObject*, const Rect&, function<void(MapObject*)>)
    
// インスタンス変数
private:
    int _power { 1 };
    function<void(MapObject*)> _onAttackHitted { nullptr };
    
// インスタンスメソッド
private:
    AttackBox();
    virtual ~AttackBox();
    virtual bool init(MapObject* parent, Node* origin, function<void(MapObject*)> onAttackHitted);
    virtual bool init(MapObject* parent, const Rect& originRect, function<void(MapObject*)> onAttackHitted);
public:
    void setPower(int power);
    int getPower() const;
    void onAttackHitted(MapObject* hittedObject);
    
// Debug
private:
    virtual Color4F getDebugMaskColor() const override;
};

#endif /* AttackBox_h */
