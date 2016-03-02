//
//  EndingScene.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/03.
//
//

#ifndef EndingScene_h
#define EndingScene_h

#include "Scenes/BaseScene.h"

class EndingScene : public BaseScene
{
// 定数
public:
    enum struct END_ID
    {
        BAD_END = 1,
        NORMAL_END = 2,
        TRUE_END = 3
    };
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EndingScene, const int)
private:
    EndingScene();
    ~EndingScene();
    
// インスタンス変数
private:
    int end_id {0};
    
// インスタンスメソッド
private:
    bool init(const int endingId);
    virtual void onEnter() override;
    virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    void createTrueEnding();
    void createNormalEnding();
    void createBadEnding();
    void onEndingFinished();
    void replaceScene();
    
};

#endif /* EndingScene_h */
