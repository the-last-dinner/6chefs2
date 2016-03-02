//
//  StartUpScene.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/13.
//
//

#ifndef StartUpScene_h
#define StartUpScene_h

#include "Scenes/BaseScene.h"

class StartUpScene : public BaseScene
{
    // クラスメソッド
public:
    CREATE_FUNC(StartUpScene);
    
    // インスタンスメソッド
private:
    virtual bool init();
    virtual void onEnter();
    void onPreloadFinished(LoadingLayer* loadingLayer);
    void encryptSaveFiles();
    void encryptEventScripts();
    void ecnryptCsvFiles();

public:
    StartUpScene() {FUNCLOG};
    ~StartUpScene() {FUNCLOG};
};

#endif /* StartUpScene_h */
