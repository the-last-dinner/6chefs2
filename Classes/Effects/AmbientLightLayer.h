//
//  AmbientLightLayer.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/10.
//
//

#ifndef _AmbientLightLayer__
#define _AmbientLightLayer__

#include "Common.h"

#include "Effects/Light.h"

class Light;

class AmbientLightLayer : public Layer
{
// 定数
public:
    static const Color3B DAY;
    static const Color3B EVENING;
    static const Color3B ROOM;
	static const Color3B NIGHT;
    static const Color3B BASEMENT;
    static const Color3B MIDNIGHT;
    
// クラスメソッド
public:
	CREATE_FUNC_WITH_PARAM(AmbientLightLayer, const Color3B&)
    
// インスタンス変数
private:
    Sprite* ambientLight { nullptr };
    map<Light*, Light*> lightMap {};
    RenderTexture* renderTexture { nullptr };

// インスタンスメソッド
public:
    void setAmbient(const Color3B& color);
    void addLightSource(Light* innerLight, Light* outerLight);
    void removeLightSource(Light* lightSource);
    void changeLightAngleTo(Light* lightSource, const float& angle);
    void update(float delta) override;
    virtual void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) override;
private:
    AmbientLightLayer();   // コンストラクタ
    ~AmbientLightLayer();  // デストラクタ
    bool init(const Color3B& color);    // 初期化
    void onEnter() override;
    void onExit() override;
};

#endif /* defined(_AmbientLightLayer__) */
