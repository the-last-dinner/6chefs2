//
//  NotificationBand.hpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/27.
//
//

#ifndef NotificationBand_h
#define NotificationBand_h

#include "define.h"

class NotificationBand : public Node
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(NotificationBand, const string&)
private:
    NotificationBand(){FUNCLOG};
    ~NotificationBand(){FUNCLOG};
    
// インスタンス変数
private:
    Sprite* band {nullptr};
    Label* messageLabel {nullptr};
    
// インスタンスメソッド
private:
    bool init(const string& message);
public:
    void setBandColor(const Color3B& color);
    void setMessageColor(const Color3B& color);
    void show(const function<void()>& callback = nullptr);
    void hide(const function<void()>& callback = nullptr);
};

#endif /* NotificationBand_h */
