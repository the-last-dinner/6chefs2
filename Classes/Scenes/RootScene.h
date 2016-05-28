//
//  RootScene.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/14.
//
//

#ifndef RootScene_h
#define RootScene_h

#include "define.h"

class ConfigEventListenerLayer;
class EventListenerKeyboardLayer;

class RootScene : public Scene
{
// クラスメソッド
public:
    CREATE_FUNC(RootScene)
    
// インスタンス変数
private:
    ConfigEventListenerLayer* configListener { nullptr };
    EventListenerKeyboardLayer* listener { nullptr };
    Sprite* cover { nullptr };
    
// インスタンスメソッド
private:
    RootScene();
    virtual ~RootScene();
    virtual bool init() override;
    virtual void onEnter() override;
public:
    ConfigEventListenerLayer* getConfigEventListener() const;
    EventListenerKeyboardLayer* getEventListenerKeyboard() const;
    Sprite* getCover() const;
public:
    void setCover(Sprite* cover);
};

#endif /* RootScene_h */
