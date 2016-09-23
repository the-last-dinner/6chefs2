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
    CREATE_FUNC_WITH_THREE_PARAM(HitBox, MapObject*, Node*, function<void()>)
    CREATE_FUNC_WITH_THREE_PARAM(HitBox, MapObject*, const Rect&, function<void()>)
    
// インスタンス変数
private:
    function<void()> _onHitted { nullptr };
    
// インスタンスメソッド
private:
    HitBox();
    virtual ~HitBox();
    virtual bool init(MapObject* parent, Node* origin, function<void()> onHitted);
    virtual bool init(MapObject* parent, const Rect& originRect, function<void()> onHitted);
    
public:
    void onHitted();
};

#endif /* HitBox_h */
