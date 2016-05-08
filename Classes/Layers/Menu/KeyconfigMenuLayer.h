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
// エイリアス
public:
    using AnimationCallback = function<void(KeyconfigMenuLayer*)>;
    
// 定数
private:
    enum struct MenuType
    {
        CURSOR,
        DASH,
        SAVE,
        CANCEL,
        
        SIZE,
    };
    static const map<MenuType, int> idNums;
    static const int RIGHT_ARROW_TAG;
    static const int LEFT_ARROW_TAG;
    
// クラスメソッド
public:
    CREATE_FUNC(KeyconfigMenuLayer)
    
// インスタンス変数
private:
    map<MenuType, int> configIdxs {};
    Sprite* cover {nullptr};
    Node* container {nullptr};
    Node* arrows { nullptr };
public:
    AnimationCallback onClose { nullptr };
    
// インスタンスメソッド
private:
    virtual bool init() override;
    virtual void onCursorKeyPressed(const Key& key) override;
    virtual void onIndexChanged(int newIdx, bool sound) override;
    virtual void onEnterKeyPressed(int idx) override;
    virtual void onMenuKeyPressed() override;
protected:
    KeyconfigMenuLayer();
    ~KeyconfigMenuLayer();
public:
    virtual void show() override;
    virtual void hide() override;
    virtual void showWithAnimation(AnimationCallback);
    virtual void hideWithAnimation(AnimationCallback);
    
private:
    void onConfigIdxChanged(MenuType type, int newIdx);
    void close();
    Sprite* createFrame(const Size& size, int idx) const;
    void setConfigNameLabel(const MenuType type, int idx);
    Color3B getFrameColor(int idx) const;
    Point calcItemPosition(Sprite* item, int idx) const;
    Point calcConfigNamePosition(Label* label, Node* frame, float centerMargin) const;
};

#endif /* KeyconfigMenuLayer_h */
