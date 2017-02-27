//
//  TitleScene.h
//  LastSupper
//
//  Created by Kohei on 2015/06/05.
//
//

#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "Scenes/BaseScene.h"

class TitleMainMenuLayer;
class SaveDataSelector;
class TrophyListLayer;

class TitleScene : public BaseScene
{
// 列挙型
private:
	enum struct MenuType
	{
		BEGINING,
		FINISH,
		SIZE,
	};

// クラスメソッド
public:
	CREATE_FUNC(TitleScene);
    
// インスタンス変数
private:
    TitleMainMenuLayer* mainMenu { nullptr };
    SaveDataSelector* saveDataSelector { nullptr };
    TrophyListLayer* trophyList { nullptr };
	
// インスタンスメソッド
private:
    TitleScene();
    ~TitleScene();
	virtual bool init() override;
    virtual void onEnter() override;
	void onPreloadFinished(LoadingLayer* loadingLayer) override;
	void onStartSelected();
	void onContinueSelected();
	void onExitSelected();
	void onSaveDataSelectCancelled();
    void onSpecialRoomSelected();
    
    // トロフィーリスト
    void createTrophyListLayer();
    void onTrophyListSelected();
    void onTrophyListCanceled();
// インターフェース
protected:
    LoadingLayer* createLoadingLayer() override;
};

#endif // __TITLE_SCENE_H__
