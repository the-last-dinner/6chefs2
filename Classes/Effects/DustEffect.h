//
//  DustEffect.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/21.
//
//

#ifndef DustEffect_h
#define DustEffect_h

#include "define.h"

class CSNode;

class DustEffect : public Node
{
// 定数
private:
    static const string CSB_FILE_PATH;
    static const string ANIMATION_NAME;
    
// クラスメソッド
public:
    CREATE_FUNC(DustEffect);
    
// インスタンス変数
private:
    CSNode* _csNode { nullptr };

// インスタンスメソッド
private:
    DustEffect();
    ~DustEffect();
    virtual bool init() override;
public:
    void animate(function<void(DustEffect*)> callback);
};

#endif /* DustEffect_h */
