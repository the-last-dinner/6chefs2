//
//  DungeonMainMenuLayer.h
//  LastSupper
//
//  Created by Kohei on 2015/09/06.
//
//

#ifndef __LastSupper__DungeonMainMenuLayer__
#define __LastSupper__DungeonMainMenuLayer__

#include "Layers/Menu/MenuLayer.h"

class MiniSelector;
class SlideNode;

class DungeonMainMenuLayer : public MenuLayer
{
	// 列挙型
public:
	enum struct Type
	{
		ITEM,
		SAVE,
		CHARA,
		TITLE,
		CLOSE,
		SIZE
	};

    // 定数
public:
    static const float SLIDE_TIME;
    static const int PARTY_DISPLAY_LIMIT;
	
	// クラスメソッド
public:
	CREATE_FUNC(DungeonMainMenuLayer)

    //インスタンス変数
public:
    function<void()> onMenuHidden {nullptr};
    function<void()> onSaveMenuSelected {nullptr};
    function<void()> onItemMenuSelected {nullptr};
    function<void()> onCharacterMenuSelected {nullptr};
    function<void()> onBackToTitleConfirmed {nullptr};
    vector<SlideNode*> slideNodes {};
    
private:
    int menuIndex {0};
    MiniSelector* confirm {nullptr};
    Label* play_time {nullptr};
    Sprite* cover {nullptr};
    Node* cursor { nullptr };
	// インスタンスメソッド
private:
	DungeonMainMenuLayer();
	~DungeonMainMenuLayer();
	virtual bool init();
    void confirmTitleback();

	virtual void onIndexChanged(int newIdx, bool sound = true);
	virtual void onEnterKeyPressed(int idx);
	virtual void onMenuKeyPressed();
    void onConfirmSelected(int idx);
    void onConfirmCanceled();
    void slideIn();
    void slideOut();
    void updateTime(float delta);
public:
    int getMenuIndex();
    void setDefaultMenuIndex(const int idx);
    virtual void show();
    virtual void hide();
};

#endif /* defined(__LastSupper__DungeonMainMenuLayer__) */
