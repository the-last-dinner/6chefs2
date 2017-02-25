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
// 列挙型
public:
    enum struct Type
    {
        NORMAL,
        TORCH,
        FLASHLIGHT,
        
        SIZE
    };
    
// 構造体
public:
    struct Information
    {
        Color3B color {Color3B::WHITE};
        float radius {0};
        Light::Type type {Light::Type::NORMAL};
        string image {};
        Information(){};
        Information(float radius):radius(radius){};
        Information(const Color3B color, float radius, Light::Type type):color(color), radius(radius), type(type){};
        Information(const Color3B color, float radius, string image):color(color), radius(radius), image(image){};
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
