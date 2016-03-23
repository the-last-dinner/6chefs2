//
//  ConfigEventListenerLayer.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/03/23.
//
//

#ifndef ConfigEventListenerLayer_h
#define ConfigEventListenerLayer_h

#include "define.h"

class ConfigEventListenerLayer : public Layer
{
// クラスメソッド
public:
    CREATE_FUNC(ConfigEventListenerLayer)
    
// インスタンス変数
private:
    bool keyconfigEnabled { true };
public:
    function<void()> onOpenkeyconfigMenu { nullptr };
    function<void()> onKeyconfigMenuClosed { nullptr };
    
// インスタンスメソッド
private:
    ConfigEventListenerLayer();
    virtual ~ConfigEventListenerLayer();
    bool init();
    void onKeyPressed(const EventKeyboard::KeyCode& keyCode);
    void switchWinSize();
    void openKeyconfigMenu();
public:
    void setKeyconfigEnabled(bool enabled);
};

#endif /* ConfigEventListenerLayer_h */
