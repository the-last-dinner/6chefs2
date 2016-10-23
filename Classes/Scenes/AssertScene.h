//
//  AssertScene.h
//  6chefs2
//
//  Created by S.inami on 2016/07/13.
//
//

#ifndef AssertScene_h
#define AssertScene_h

#include "Scenes/BaseScene.h"

class EventListenerKeyboardLayer;

class AssertScene : public BaseScene
{
    // 定数
public:
    enum struct AssertType
    {
        INFO, // デバッグ用お知らせ
        WARNING, // 修正必要な警告
        FATAL, // 致命的なエラー
        ERROR // ユーザー出力用
    };

    // クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(AssertScene, string, AssertType);
    
    // インスタンス変数
public:
    function<void()> onEnterPushedScene {nullptr};
    function<void()> onExitPushedScene {nullptr};
    
protected:
    EventListenerKeyboardLayer* listenerKeyboard { nullptr };
 
    // インスタンスメソッド
private:
    AssertScene();
    ~AssertScene();
    bool init(const string& message, const AssertType& assertType);
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    virtual void onCursorKeyPressed(const Key& key);
    virtual void onEnterKeyPressed();
    virtual void onMenuKeyPressed();
    virtual void intervalInputCheck(const vector<Key>& keys);
};

#endif /* AssertScene_h */
