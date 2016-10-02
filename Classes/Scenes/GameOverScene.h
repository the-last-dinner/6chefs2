//
//  GameOverScene.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/02.
//
//

#ifndef GameOverScene_h
#define GameOverScene_h

#include "Scenes/BaseScene.h"

class GameOverScene : public BaseScene
{
// 定数
public:
    enum struct Type
    {
        BLOOD,
        
        SIZE,
    };
    
    static const float CHAR_ANIMATION_DURATION;
    static const float CHAR_ANIMATION_LATENCY;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(GameOverScene, const Type)
    
// インスタンス変数
public:
    Sprite* _bloodCover { nullptr };
    
// インスタンスメソッド
private:
    GameOverScene();
    ~GameOverScene();
    bool init(const Type type);
    virtual void onEnter() override;
    virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    void onAnimationFinished();
};

#endif /* GameOverScene_h */
