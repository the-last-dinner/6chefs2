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
    CREATE_FUNC_WITH_TWO_PARAM(EndingScene, const int, function<void()>)
private:
    EndingScene();
    ~EndingScene();
    
// インスタンス変数
private:
    int end_id {0};
    function<void()> onfinished { nullptr };
// インスタンスメソッド
private:
    bool init(const int endingId, function<void()> onfinished);
    virtual void onEnter() override;
    virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    void createSpecialEnding(rapidjson::Value& json);
    void createNormalEnding(rapidjson::Value& json);
    void onEndingFinished();
};

#endif /* EndingScene_h */
