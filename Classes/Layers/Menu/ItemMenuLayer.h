//
//  ItemMenuLayer.hpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/01.
//
//

#ifndef ItemMenuLayer_h
#define ItemMenuLayer_h

#include "Layers/Menu/MenuLayer.h"

class MiniSelector;
class NotificationBand;

class ItemMenuLayer : public MenuLayer
{
    // クラスメソッド
public:
    CREATE_FUNC(ItemMenuLayer)
    virtual bool init() override;
    
    // インスタンスメソッド
private:
    virtual void onIndexChanged(int newIdx, bool sound) override;
    virtual void onEnterKeyPressed(int idx) override;
    virtual void onMenuKeyPressed() override;
    virtual void onPageChanged(const int page) override;
    void changeItemDiscription(const int idx);
    void createMiniSelector();
    void onMiniSelectorCanceled();
    void onMiniIndexSelected(const int idx);
protected:
    ItemMenuLayer();
    ~ItemMenuLayer();
public:
    virtual void show() override;
    virtual void hide() override;
    
    // インスタンス変数
private:
    vector<int> items {};
    MiniSelector* miniSelector {nullptr};
    NotificationBand* notification {nullptr};
    int selected_item {0};
    bool isEquip {true};
public:
    function<void()> onItemMenuCanceled { nullptr };
};
#endif /* ItemMenuLayer_h */
