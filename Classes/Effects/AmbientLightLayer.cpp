//
//  AmbientLightLayer.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/10.
//
//

#include "Effects/AmbientLightLayer.h"

#include "MapObjects/MapObject.h"

// 定数
const Color3B AmbientLightLayer::DAY {Color3B(Color4F(1.f, 1.f, 0.95f, 1.f))};
const Color3B AmbientLightLayer::EVENING {Color3B(Color4F(1.f, 0.4f, 0.f, 1.f))};
const Color3B AmbientLightLayer::ROOM {Color3B(Color4F(0.48f, 0.48f, 0.65f, 1.f))};
const Color3B AmbientLightLayer::NIGHT {Color3B(Color4F(0.36f, 0.36f, 0.70f, 1.f))};
const Color3B AmbientLightLayer::BASEMENT {Color3B(Color4F(0.10f, 0.10f, 0.20f, 1.f))};
const Color3B AmbientLightLayer::MIDNIGHT {Color3B(Color4F(0.15f, 0.15f, 0.35f, 1.f))};

// コンストラクタ
AmbientLightLayer::AmbientLightLayer(){ FUNCLOG }

// デストラクタ
AmbientLightLayer::~AmbientLightLayer()
{
    FUNCLOG

    for(pair<Light*, Light*> element : this->lightMap)
    {
        if(!element.first) continue;
        
        CC_SAFE_RELEASE_NULL(element.first);
    }
}

// 初期化
bool AmbientLightLayer::init(const Color3B& color)
{
    if(!Layer::init()) return false;
    
    // 画面サイズ取得
    Size winSize { Director::getInstance()->getWinSize() };
    
    //環境光Sprite生成
    Sprite* ambientSprite {Sprite::create()};
    ambientSprite->setTextureRect(Rect(0, 0, winSize.width, winSize.height));
    ambientSprite->setColor(color);
    ambientSprite->setPosition(winSize / 2);
    this->addChild(ambientSprite);
    this->ambientLight = ambientSprite;
    
    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);
    
    // レンダーテクスチャを生成
    RenderTexture* renderTexture { RenderTexture::create(winSize.width, winSize.height) };
    this->addChild(renderTexture);
    this->renderTexture = renderTexture;
    
    // レンダーテクスチャを反映させるためのスプライトを生成
    Sprite* renderTexSprite {Sprite::create()};
    renderTexSprite->setTexture(renderTexture->getSprite()->getTexture());
    renderTexSprite->setTextureRect(renderTexture->getSprite()->getTextureRect());
    renderTexSprite->setPosition(WINDOW_CENTER);
    renderTexSprite->setFlippedY(true);
    this->addChild(renderTexSprite);
    this->renderTexSprite = renderTexSprite;
    
    renderTexSprite->setBlendFunc(BlendFunc{GL_ZERO, GL_SRC_COLOR});
    
    return true;
}

// 環境光の色を設定
void AmbientLightLayer::setAmbient(const Color3B& color)
{
    this->ambientLight->setColor(color);
}

// 光源を追加
void AmbientLightLayer::addLightSource(Light* lightSource)
{
    if(!lightSource) return;
    
    Light::Information info {lightSource->getInformation()};
    
    CC_SAFE_RETAIN(lightSource);
    Color3B color { Color3B(info.color.r * 1.3f, info.color.g * 1.3f, info.color.b * 1.3f)};
    float radius {info.radius * 7.0f};
    
    Light* light {Light::create(Light::Information(color, radius, info.type))};
    light->setPosition(lightSource->convertToWorldSpace(lightSource->getPosition()));
    light->setOpacity(0);
    this->addChild(light);
    
    float duration {0.5f};
    light->runAction(FadeIn::create(duration));
    
    this->lightMap.insert({lightSource, light});
}

// 光源を削除
void AmbientLightLayer::removeLightSource(Light* lightSource)
{
    if(!lightSource) return;
    
    if(this->lightMap.count(lightSource) == 0) return;
    Light* light {this->lightMap.at(lightSource)};
    light->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), CallFunc::create([this, lightSource](){this->lightMap.erase(lightSource); CC_SAFE_RELEASE(lightSource);}), nullptr));
}

void AmbientLightLayer::visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    // 光の状態、位置を更新
    for(pair<Light*, Light*> element : this->lightMap)
    {
        Light* lightSource {element.first};
        Light* light {element.second};
        if(lightSource->getReferenceCount() == 1)
        {
            this->removeLightSource(lightSource);
            
            continue;
        }
        
        light->setPosition(lightSource->convertToWorldSpace(lightSource->getPosition()));
    }
    
    this->renderTexture->beginWithClear(0, 0, 0, 0);
    for (auto child : this->getChildren())
    {
        if (child != this->renderTexture && child != this->renderTexSprite)
            child->visit(renderer, parentTransform, parentFlags);
    }
    this->renderTexture->end();
    
    this->renderTexSprite->visit(renderer, parentTransform, parentFlags);
}