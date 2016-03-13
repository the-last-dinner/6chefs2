//
//  NumberSelector.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/29.
//
//

#ifndef NumberSelector_h
#define NumberSelector_h

#include "Layers/Menu/MenuLayer.h"

class NumberSelector : public MenuLayer
{
    // クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(NumberSelector, SpriteUtils::Square&)
    virtual bool init(SpriteUtils::Square&);
    
    // インスタンス変数
public:
    function<void()> onNumberSelectorCanceled { nullptr };
    function<void(int index)> onNumberSelectorSelected {nullptr};
    
    // インスタンスメソッド
private:
    virtual void onIndexChanged(int newIdx, bool sound);
    virtual void onEnterKeyPressed(int idx);
    virtual void onMenuKeyPressed();
    int indexToLabel(int idx);
protected:
    NumberSelector() {FUNCLOG};
    ~NumberSelector() {FUNCLOG};
public:
    virtual void show() override;
    virtual void hide() override;
};

#endif /* NumberSelector_h */
