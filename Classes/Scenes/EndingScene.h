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
private:
    static const string ENDING_FILE;
    static const char* JSON_MEMBER_TYPE;
    static const char* JSON_MEMBER_TEXT;
    static const char* JSON_MEMBER_CREDITS;
    static const char* JSON_MEMBER_TIME;
    static const char* JSON_MEMBER_PICTURES;
    static const char* JSON_MEMBER_NAME;
    static const char* JSON_MEMBER_BACKGROUND;
    static const char* JSON_MEMBER_LAST_TEXT;
    static const char* JSON_MEMBER_LAST_PICTURE;
    static const char* JSON_MEMBER_BGM;
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(EndingScene, const int)
private:
    EndingScene();
    ~EndingScene();
    
// インスタンス変数
public:
    function<void()> onEnterPushedScene {nullptr};
    function<void()> onExitPushedScene {nullptr};
private:
    int _end_id {0};
// インスタンスメソッド
private:
    bool init(const int endingId);
    virtual void onEnter() override;
     virtual void onExit() override;
    virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    void createSpecialEnding(rapidjson::Value& json);
    void createNormalEnding(rapidjson::Value& json);
    void onEndingFinished();
};

#endif /* EndingScene_h */
