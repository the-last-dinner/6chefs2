//
//  ItemObject.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#ifndef ItemObject_h
#define ItemObject_h

#include "MapObjects/MapObject.h"

class ItemObject : public MapObject
{
// 定数
private:
    static const string PREFIX;
    static const string BLUE;
    static const int ANIMATION_FRAME_NUM;
    static const float ANIMATION_DELAY_PER_UNIT;
    
// クラスメソッド
public:
    CREATE_FUNC(ItemObject)

// インスタンス変数
private:
    Action* action { nullptr };
    
// インスタンスメソッド
public:
    ItemObject();
    ~ItemObject();
    bool init();
    virtual void onEnterMap() override;
    string getSpriteFrameName(int state);
};

#endif /* ItemObject_h */
