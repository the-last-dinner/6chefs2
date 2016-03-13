//
//  CharacterMenuLayer.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/09.
//
//

#ifndef CharacterMenuLayer_h
#define CharacterMenuLayer_h

#include "Layers/Menu/MenuLayer.h"

class SlideNode;

class CharacterMenuLayer : public MenuLayer
{
    // クラスメソッド
public:
    CREATE_FUNC(CharacterMenuLayer)
    virtual bool init() override;
  
    // インスタンスメソッド
private:
    virtual void onIndexChanged(int newIdx, bool sound) override;
    virtual void onEnterKeyPressed(int idx) override;
    virtual void onMenuKeyPressed() override;
    void changeCharaImage(const int idx);
protected:
    CharacterMenuLayer();
    ~CharacterMenuLayer();
public:
    virtual void show() override;
    virtual void hide() override;
    
    // インスタンス変数
private:
    vector<int> characters {};
    int selected_character {0};
    bool isDiscription {false};
    SlideNode* slideNode {nullptr};
public:
    function<void()> onCharacterMenuCanceled { nullptr };
};
#endif /* CharacterMenuLayer_h */
