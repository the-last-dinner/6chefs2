//
//  SaveDataSelector.h
//  LastSupper
//
//  Created by Kohei on 2015/07/20.
//
//

#ifndef __SAVE_DATA_SELECTOR_H__
#define __SAVE_DATA_SELECTOR_H__

#include "Layers/Menu/MenuLayer.h"

class NotificationBand;
struct CharacterData;

class SaveDataSelector : public MenuLayer
{
    // 構造体
private:
    struct SaveIndex
    {
        int dataId {0};
        string chapter {};
        string mapName {};
        string playTime {};
        string saveCount {};
        int friendship {0};
        vector<string> party {};
        SaveIndex(){};
    };
    
    // 定数
private:
    const static int MAX_FRIENDSHIP_COUNT;
    const static int MAX_PARTY_DISPLAY_COUNT;
    
	// クラスメソッド
public:
	CREATE_FUNC_WITH_PARAM(SaveDataSelector, bool);
	virtual bool init(bool write);
    
    // インスタンス変数
private:
    NotificationBand* notification {nullptr};
    bool existsSaveData[MAX_SAVE_COUNT] {};
    bool writeFlag;
    bool comfirmFlag;
public:
    function<void()> onSaveDataSelectCancelled { nullptr };
	
	// インスタンスメソッド
private:
    Node* createSavePanel(SaveIndex saveIndex);
    vector<SaveIndex> createSaveIndexies();
	virtual void onIndexChanged(int newIdx, bool sound) override;
	virtual void onEnterKeyPressed(int idx) override;
	virtual void onMenuKeyPressed() override;
protected:
	SaveDataSelector();
	~SaveDataSelector();
public:
	virtual void show() override;
	virtual void hide() override;
};

#endif // __SAVE_DATA_SELECTOR_H__
