//
//  MiniSelector.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/08.
//
//

#ifndef MiniSelector_h
#define MiniSelector_h

#include "Layers/Menu/MenuLayer.h"

class MiniSelector: public MenuLayer
{
public:
    // ミニ選択ウインドウ設定構造体
    struct Selector
    {
    private:
        Point index {Point(0,0)};
        Color3B color {Color3B::WHITE}; // 文字色
        Color3B background_color {Color3B::BLACK}; // 背景色
        Color3B rectangle_color {Color3B::WHITE}; // 枠色
        SpriteUtils::Square position {SpriteUtils::Square(0,0,0,0)}; // 位置
        SpriteUtils::Margin margin {SpriteUtils::Margin(3.0)}; // margin
        vector<string> menu {}; // menuのインデックス
    public:
        Selector(const Point& index, const SpriteUtils::Square& position, const vector<string>& menu):index(index),position(position),menu(menu){}
        Selector(const Point& index, const SpriteUtils::Square& position, const vector<string>& menu, const SpriteUtils::Margin& margin):index(index),position(position),menu(menu),margin(margin){}
        void setColor(const Color3B& color){this->color = color;};
        void setBackgroundColor(const Color3B& color){this->background_color = color;};
        void setRectangleColor(const Color3B& color){this->rectangle_color = color;};
        const Point getIndexSize(){return index;};
        const Size getWindowSize(){return Size(position.getWidth(),position.getHeight());};
        const SpriteUtils::Square getPosition(){return position;};
        const SpriteUtils::Margin getMargin(){return margin;};
        const vector<string> getMenu(){return menu;};
        const Color3B getColor(){return color;};
        const Color3B getBackgroundColor(){return background_color;};
        const Color3B getRectangleColor(){return rectangle_color;};
    };
    
    // 画像からミニ選択ウインドウを設定する構造体
    struct SelectorWithSprite
    {
    private:
        Point index {Point(0,0)};
        Sprite* window {};
        vector<string> menu {}; // menuのインデックス
        Color3B color {Color3B::WHITE};
    public:
        SelectorWithSprite(const Point& index, Sprite* window, const vector<string>& menu):index(index),window(window),menu(menu){};
        void setColor(const Color3B& color){this->color = color;};
        const Point getIndexSize(){return index;};
        const vector<string> getMenu(){return menu;};
        Sprite* getWindow(){return window;};
        Color3B getColor(){return color;};
        
    };
    
    // クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(MiniSelector, Selector&)
    CREATE_FUNC_WITH_PARAM(MiniSelector, SelectorWithSprite&)
    virtual bool init(Selector& selector);
    virtual bool init(SelectorWithSprite& selector);
    
    // インスタンス変数
public:
    bool confirm_flag {false};
    function<void()> onMiniSelectorCanceled { nullptr };
    function<void(int index)> onMiniIndexSelected {nullptr};
    Sprite* window {nullptr};
    
    // インスタンスメソッド
private:
    virtual void onIndexChanged(int newIdx, bool sound) override;
    virtual void onEnterKeyPressed(int idx);
    virtual void onMenuKeyPressed() override;
protected:
    MiniSelector();
    ~MiniSelector();
public:
    virtual void show() override;
    virtual void hide() override;
};

#endif /* MiniSelector_h */
