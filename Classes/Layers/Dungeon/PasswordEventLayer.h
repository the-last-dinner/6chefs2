//
//  PasswordEventLayer.h
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/29.
//
//

#ifndef PasswordEventLayer_h
#define PasswordEventLayer_h

#include "Layers/Menu/MenuLayer.h"

class NumberSelector;

class PasswordEventLayer : public Layer
{
    // 定数
public:
    enum struct Result
    {
        SUCCESS,
        FAILURE,
        
        SIZE,
    };
    using ResultCallback = function<void(Result)>;
    
    // クラスメソッド
public:
    static PasswordEventLayer* create(const string& password, ResultCallback callback);
    
    // インスタンス変数
private:
    bool checking {false};
    string password {};
    int pass_length {1};
    ResultCallback callback { nullptr };
    vector<string> inputed {};
    vector<Label*> confirmLabels {};
    
    // インスタンスメソッド
private:
    PasswordEventLayer();
    ~PasswordEventLayer();
    bool init(const string& password, ResultCallback callback);
    void onNumberSelected(int num);
    void onNumberSelectCanceled();
    void show();
    void hide();
};

#endif /* PasswordEventLayer_h */