//
//  DungeonMenuScene.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/10/04.
//
//

#ifndef __LastSupper__DungeonMenuScene__
#define __LastSupper__DungeonMenuScene__

#include "Scenes/BaseScene.h"

class DungeonMainMenuLayer;
class SaveDataSelector;
class ItemMenuLayer;
class CharacterMenuLayer;

class DungeonMenuScene : public BaseScene
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(DungeonMenuScene, Sprite*);
    
// クラス変数
private:
    static const string MAIN_LAYER_NAME;
    static const string SAVE_LAYER_NAME;
    static const string ITEM_LAYER_NAME;
    static const string CHARA_LAYER_NAME;

// インスタンス変数
public:
    function<void()> onPopMenuScene {nullptr};
    function<void()> onBackToTitleSelected {nullptr};
private:
    DungeonMainMenuLayer* mainMenu {nullptr};
    SaveDataSelector* saveDataSelector {nullptr};
    ItemMenuLayer* itemMenu {nullptr};
    CharacterMenuLayer* charaMenu {nullptr};
    int menuIndex {0};
    
// インスタンスメソッド
public:
    void onMenuHidden();
private:
    DungeonMenuScene();
    ~DungeonMenuScene();
    bool init(Texture2D* screen);
    bool init(Sprite* screen);
    virtual void onEnter() override { Scene::onEnter();};
    virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    virtual void onCursorKeyPressed(const Key& key);
    virtual void onEnterKeyPressed();
    virtual void onMenuKeyPressed();
    virtual void intervalInputCheck(const vector<Key>& keys);
    void createMainMenu();
    void createSaveMenu();
    void createItemMenu();
    void createCharaMenu();
    // メインメニュー
    void onSaveMenuSelected();
    void onItemMenuSelected();
    void onCharaMenuSelected();
    // セーブデータセレクター
    void onSaveDataSelectCancelled();
    // アイテムメニュー
    void onItemMenuCanceled();
    // キャラクターメニュー
    void onCharaMenuCanceled();
    // タイトルへ戻るを承認した時
    void onBackToTitleConfirmed();
};

#endif /* defined(__LastSupper__DungeonMenuScene__) */
