//
//  Light.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/13.
//
//

#ifndef _Light__
#define _Light__

#include "Common.h"

class Light : public Node
{
// 定数
public:
    static const Color3B TORCH_COLOR;
    
// 列挙型
public:
    enum struct Type
    {
        NORMAL,
        TORCH,
        
        SIZE
    };
    
// 構造体
public:
    struct Information
    {
        Color3B color {TORCH_COLOR};
        float radius {0};
        Type type {Type::TORCH};
        Information(){};
        Information(float radius):radius(radius){};
        Information(const Color3B color, float radius, const Type& type):color(color), radius(radius), type(type){};
    };
    
// クラスメソッド
public:
	CREATE_FUNC_WITH_PARAM(Light, const Information&);
	
// インスタンス変数
private:
    Information info {};
    Sprite* sprite { nullptr };

// インスタンスメソッド
public:
    Light();   // コンストラクタ
    ~Light();  // デストラクタ
	bool init(const Information& info);						  // 初期化
    Information getInformation();
    void setBlendFunc(const BlendFunc& blendFunc);
};

#endif /* defined(_Light__) */
