//
//  DocumentMenuLayer.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/09.
//
//

#ifndef DocumentMenuLayer_h
#define DocumentMenuLayer_h

#include "Layers/Menu/MenuLayer.h"

class SlideNode;

class DocumentMenuLayer : public MenuLayer
{
    // クラスメソッド
public:
    CREATE_FUNC(DocumentMenuLayer)
    virtual bool init() override;
  
    // インスタンスメソッド
private:
    virtual void onIndexChanged(int newIdx, bool sound) override;
    virtual void onEnterKeyPressed(int idx) override;
    virtual void onMenuKeyPressed() override;
    void changeCharaImage(const int idx);
protected:
    DocumentMenuLayer();
    ~DocumentMenuLayer();
public:
    virtual void show() override;
    virtual void hide() override;
    
    // インスタンス変数
private:
    vector<int> documentIds {};
    bool isDiscription {false};
    SlideNode* slideNode {nullptr};
public:
    function<void()> onCharacterMenuCanceled { nullptr };
};
#endif /* DocumentMenuLayer_h */
