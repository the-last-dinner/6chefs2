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
    // クラスメソッド
public:
    CREATE_FUNC_WITH_THREE_PARAM(AssertScene, string, string, bool);
    
    // インスタンス変数
public:
    function<void()> onPushAssertScene {nullptr};
    function<void()> onPopAssertScene {nullptr};
    
protected:
    EventListenerKeyboardLayer* listenerKeyboard { nullptr };
 
    // インスタンスメソッド
private:
    AssertScene();
    ~AssertScene();
    bool init(const string& title, const string& message, const bool& hideable);
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    virtual void onCursorKeyPressed(const Key& key);
    virtual void onEnterKeyPressed();
    virtual void onMenuKeyPressed();
    virtual void intervalInputCheck(const vector<Key>& keys);
};

#endif /* AssertScene_h */
