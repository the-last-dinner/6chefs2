//
//  KeyconfigMenuLayer.h
//  6chef2
//
//  Created by Kohei Asami on 2016/03/12.
//
//

#ifndef KeyconfigMenuLayer_h
#define KeyconfigMenuLayer_h

#include "Layers/Menu/MenuLayer.h"

class KeyconfigMenuLayer : public MenuLayer
{
// クラスメソッド
public:
    CREATE_FUNC(KeyconfigMenuLayer)
    
// インスタンスメソッド
private:
    virtual bool init() override;
    virtual void onIndexChanged(int newIdx, bool sound) override {};
    virtual void onSpacePressed(int idx) override {};
    virtual void onMenuKeyPressed() override {};
protected:
    KeyconfigMenuLayer();
    ~KeyconfigMenuLayer();
public:
    virtual void show() override {};
    virtual void hide() override {};
};

#endif /* KeyconfigMenuLayer_h */
