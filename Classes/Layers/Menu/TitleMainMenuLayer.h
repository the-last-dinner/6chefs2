//
//  TitleMainMenuLayer.h
//  LastSupper
//
//  Created by Kohei on 2015/07/05.
//
//

#ifndef __TITLE_MAIN_MENU_LAYER_H__
#define __TITLE_MAIN_MENU_LAYER_H__

#include "Layers/Menu/MenuLayer.h"

class NotificationBand;

class TitleMainMenuLayer : public MenuLayer
{
// 定数
public:
	enum struct MenuType
	{
		START,
		CONTINUE,
        TROPHY,
        SPECIAL_ROOM,
		EXIT,
        
		SIZE
	};
	
// クラスメソッド
public:
	CREATE_FUNC(TitleMainMenuLayer);
	
// インスタンスメソッド
private:
	TitleMainMenuLayer();
	~TitleMainMenuLayer();
	virtual bool init() override;
    void onEnterAnimationFinished();
	virtual void onIndexChanged(int newIdx, bool sound = false) override;
	virtual void onEnterKeyPressed(int idx) override;
	virtual void onMenuKeyPressed() override {};
    void prohibitNotification(const string& msg);
public:
	virtual void show() override;
	virtual void hide() override;
	
// インスタンス変数
private:
    NotificationBand* notification {nullptr};
    string specialRoomTitle {};
public:
    bool animating { true };
    Node* cursor { nullptr };
	function<void()> onStartSelected { nullptr };
	function<void()> onContinueSelected { nullptr };
    function<void()> onTrophySelected { nullptr };
    function<void()> onSpecialRoomSelected { nullptr };
	function<void()> onExitSelected { nullptr };
};

#endif // __TITLE_MAIN_MENU_LAYER_H__
