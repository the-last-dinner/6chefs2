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

class SaveDataSelector : public MenuLayer
{
    //構造体
private:
    struct SaveIndex
    {
        int data_id {0};
        string chapter {};
        string map_name {};
        string play_time {};
        string save_count {};
        int friendship {0};
        SaveIndex(int i, const string& chap, const string& mn, const string& pt, const string& sc, const int fs):data_id(i), chapter(chap), map_name(mn), play_time(pt), save_count(sc), friendship(fs){};
        SaveIndex(){};
    };
    
	// クラスメソッド
public:
	CREATE_FUNC_WITH_PARAM(SaveDataSelector, bool);
	virtual bool init(bool write);
    
    // インスタンス変数
private:
    vector<SaveIndex> saveDatas {};
    NotificationBand* notification {nullptr};
    bool existsSaveData[MAX_SAVE_COUNT] {};
    bool write_flag;
    bool comfirm_flag;
public:
    function<void()> onSaveDataSelectCancelled { nullptr };
	
	// インスタンスメソッド
private:
    vector<SaveIndex> getSaveList();
	virtual void onIndexChanged(int newIdx, bool sound);
	virtual void onEnterKeyPressed(int idx);
	virtual void onMenuKeyPressed();
protected:
	SaveDataSelector();
	~SaveDataSelector();
public:
	virtual void show() override;
	virtual void hide() override;
};

#endif // __SAVE_DATA_SELECTOR_H__
