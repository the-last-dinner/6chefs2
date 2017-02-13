//
//  OpeningScene.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2017/02/13.
//
//

#ifndef OpeningScene_hpp
#define OpeningScene_hpp

#include "Scenes/BaseScene.h"

class OpeningScene : public BaseScene
{
    // クラスメソッド
public:
    CREATE_FUNC(OpeningScene);

    // インスタンス変数
private:
    string _videoFileName;
    string _bgmFileName;
    
    // インスタンスメソッド
private:
    virtual bool init();
    virtual void onEnter();
    void onPreloadFinished(LoadingLayer* loadingLayer);
    void setPressEnter();
    void setVideoLayer();
    void setVideoLayerDummy();
    void setVideoBgm();
    
public:
    OpeningScene() {FUNCLOG};
    ~OpeningScene() {FUNCLOG};
};

#endif /* OpeningScene_hpp */
