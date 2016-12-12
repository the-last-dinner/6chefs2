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
    CREATE_FUNC_WITH_THREE_PARAM(HitBox, MapObject*, Node*, function<void(int)>)
    CREATE_FUNC_WITH_THREE_PARAM(HitBox, MapObject*, const Rect&, function<void(int)>)
    
// インスタンス変数
private:
    function<void(int)> _onHitted { nullptr };
    
// インスタンスメソッド
private:
    HitBox();
    virtual ~HitBox();
    virtual bool init(MapObject* parent, Node* origin, function<void(int)> onHitted);
    virtual bool init(MapObject* parent, const Rect& originRect, function<void(int)> onHitted);
    
public:
    void onHitted(int damage);
    
// インターフェース
public:
    bool intersects(const DetectionBox* other) const override;

// Debug
private:
    virtual Color4F getDebugMaskColor() const override;
};

#endif /* HitBox_h */
