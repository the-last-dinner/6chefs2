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
    CREATE_FUNC_WITH_THREE_PARAM(AttackBox, MapObject*, Node*, function<void()>)
    CREATE_FUNC_WITH_THREE_PARAM(AttackBox, MapObject*, const Rect&, function<void()>)
    
// インスタンス変数
private:
    function<void()> _onAttackHitted { nullptr };
    
// インスタンスメソッド
private:
    AttackBox();
    virtual ~AttackBox();
    virtual bool init(MapObject* parent, Node* origin, function<void()> onAttackHitted);
    virtual bool init(MapObject* parent, const Rect& originRect, function<void()> onAttackHitted);
public:
    void onAttackHitted();
};

#endif /* AttackBox_h */
