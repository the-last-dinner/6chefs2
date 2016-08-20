//
//  MenuLayer.h
//  LastSupper
//
//  Created by Kohei on 2015/07/05.
//
//

#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "Common.h"

class EventListenerKeyboardLayer;

class MenuLayer : public Layer
{
// インスタンス変数
protected:
    vector<Node*> menuObjects {};
    vector<Node*> pagePanels {};
    EventListenerKeyboardLayer* listenerKeyboard { nullptr };
    Node* basePanel {};
    
private:
    int indexX { 0 };
    int indexY { 0 };
    int sizeX { 0 };
    int sizeY { 0 };
    bool cursorEnabled {true};
    int pageCount { 1 };
    int page { 0 };

// インスタンスメソッド
public:
	virtual void show() = 0;
	virtual void hide() = 0;
protected:
	MenuLayer();
	virtual ~MenuLayer();
    virtual bool init(const Point& size, const int itemCount, Node* basePanel);
	virtual bool init(const Point& index, const Size& size);
	virtual bool init(int sizeX, int sizeY);
	virtual void onIndexChanged(int newIdx, bool sound = true) = 0;
    virtual void onCursorKeyPressed(const Key& key);
    void onEnterPressed();
	virtual void onEnterKeyPressed(int idx) = 0;
	virtual void onMenuKeyPressed();
	virtual int getSelectedIndex();
    virtual void setSelectedIndex(const int index);
    virtual void onPageChanged(const int page);
	int getMenuSizeX() const;
	int getMenuSizeY() const;
    int calcSelectedIndex();
    void setCursorEnable(const bool enable);
    virtual void intervalInputCheck(const vector<Key>& keys);
    void changePage(const int page);
    int getPageSize();
    void moveUpDown(cocos2d::Node* target);
};

#endif // __MENU_LAYER_H__