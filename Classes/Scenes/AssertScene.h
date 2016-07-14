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
    CREATE_FUNC_WITH_TWO_PARAM(AssertScene, string, bool);
    
    // インスタンス変数
protected:
    EventListenerKeyboardLayer* listenerKeyboard { nullptr };
    
private:
    AssertScene();
    ~AssertScene();
    bool init(const string& message, const bool& hideable);
    virtual void onEnter() override { Scene::onEnter();};
    virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    virtual void onCursorKeyPressed(const Key& key);
    virtual void onEnterKeyPressed();
    virtual void onMenuKeyPressed();
    virtual void intervalInputCheck(const vector<Key>& keys);
};

#endif /* AssertScene_h */
