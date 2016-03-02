//
//  TrophyListLayer.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/02.
//
//

#ifndef TrophyListLayer_h
#define TrophyListLayer_h

#include "Layers/Menu/MenuLayer.h"

class TrophyListLayer : public MenuLayer
{
    // クラスメソッド
public:
    CREATE_FUNC(TrophyListLayer)
    virtual bool init();
    
    // インスタンスメソッド
private:
    virtual void onIndexChanged(int newIdx, bool sound);
    virtual void onSpacePressed(int idx);
    virtual void onMenuKeyPressed();
    void changeTrophyDiscription(const int idx);
protected:
    TrophyListLayer(){FUNCLOG};
    ~TrophyListLayer(){FUNCLOG};
public:
    virtual void show() override;
    virtual void hide() override;
    
    // インスタンス変数
private:
    vector<int> trophies {};
    int selected_trophy {0};
public:
    function<void()> onTrophyListCanceled { nullptr };
};

#endif /* TrophyListLayer_h */
