//
//  CollisionBox.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/08.
//
//

#ifndef CollisionBox_h
#define CollisionBox_h

#include "define.h"

class MapObject;

class CollisionBox : public Node
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(CollisionBox, Node*)
    CREATE_FUNC_WITH_PARAM(CollisionBox, const Rect&)
    
// インスタンス変数
private:
    Node* origin { nullptr };
    MapObject* parent { nullptr };
    
// インスタンスメソッド
private:
    CollisionBox();
    virtual ~CollisionBox();
    virtual bool init(Node* origin);
    virtual bool init(const Rect& rect);
    void setRect(const Rect& rect);
public:
    void onEnterMap();
    virtual void update(float delta) override;
};

#endif /* CollisionBox_h */
