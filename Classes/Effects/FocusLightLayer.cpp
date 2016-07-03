//
//  FocusLightLayer.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/08.
//
//

#include "Effects/FocusLightLayer.h"

#include "Effects/Light.h"

// 定数
const Color3B FocusLightLayer::AMBIENT_LIGHT_COLOR { Color3B(200, 200, 200) };

#pragma mark private

// コンストラクタ
FocusLightLayer::FocusLightLayer() { FUNCLOG };

// デストラクタ
FocusLightLayer::~FocusLightLayer()
{
    FUNCLOG
    
    for(pair<Node*, Sprite*> element : this->lightMap)
    {
        if(!element.first) continue;
        
        CC_SAFE_RELEASE_NULL(element.first);
    }
    
    if(this->renderTexture) this->renderTexture->autorelease();
    if(this->ambientSprite) this->ambientSprite->autorelease();
};

// 初期化
bool FocusLightLayer::init()
{
    if(!Layer::init()) return false;
    
    // 画面サイズ取得
    Size winSize { Director::getInstance()->getWinSize() };
    
    //環境光Sprite生成
    Sprite* ambientSprite {Sprite::create()};
    ambientSprite->setTextureRect(Rect(0, 0, winSize.width, winSize.height));
    ambientSprite->setColor(AMBIENT_LIGHT_COLOR);
    ambientSprite->setPosition(winSize / 2);
    this->addChild(ambientSprite);
    this->ambientSprite = ambientSprite;
    CC_SAFE_RETAIN(ambientSprite); // crash fix
    
    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);
    
    // レンダーテクスチャを生成
    RenderTexture* renderTexture { RenderTexture::create(winSize.width, winSize.height) };
    this->addChild(renderTexture);
    this->renderTexture = renderTexture;
    CC_SAFE_RETAIN(renderTexture); // crash fix
    
    // レンダーテクスチャ用Sprite
    Sprite* renderTexSprite { renderTexture->getSprite() };
    renderTexSprite->setPosition(WINDOW_CENTER);
    renderTexSprite->setBlendFunc(BlendFunc{GL_ZERO, GL_SRC_COLOR});
    
    return true;
}

void FocusLightLayer::onEnter()
{
    Layer::onEnter();
    this->scheduleUpdate();
}

void FocusLightLayer::onExit()
{
    Layer::onExit();
    this->unscheduleUpdate();
}

// フォーカスライトを削除
void FocusLightLayer::removeFocusLight(Node* target)
{
    if(!target) return;
    if(this->lightMap.count(target) == 0) return;
    
    Sprite* light {this->lightMap.at(target)};
    light->runAction(Sequence::create(FadeOut::create(0.3f), RemoveSelf::create(), CallFunc::create([this, target]()
    {
        this->lightMap.erase(target);
        CC_SAFE_RELEASE(target);
    }), nullptr));
}

# pragma mark -
# pragma mark public

// フォーカスするNodeを追加
void FocusLightLayer::addTarget(Node* target)
{
    if(!target) return;
    
    CC_SAFE_RETAIN(target);
    
    Sprite* light { Sprite::createWithSpriteFrameName("focus_light.png") };
    light->setPosition(target->getParent()->convertToWorldSpace(target->getPosition()));
    light->setOpacity(0);
    light->setBlendFunc(BlendFunc{GL_SRC_COLOR, GL_ONE});
    this->ambientSprite->addChild(light);
    
    light->runAction(FadeIn::create(0.3f));
    
    this->lightMap.insert({target, light});
}

void FocusLightLayer::update(float delta)
{
    // 光の状態、位置を更新
    for(pair<Node*, Sprite*> element : this->lightMap)
    {
        Node* target {element.first};
        Sprite* light {element.second};
        if(target->getReferenceCount() == 1)
        {
            this->removeFocusLight(target);
            
            continue;
        }
        if (target->getParent()) {
            light->setPosition(target->getParent()->convertToWorldSpace(target->getPosition()));
        }
    }
    
    // レンダーテクスチャに焼き込み
    this->renderTexture->beginWithClear(0.f, 0.f, 0.f, 0.f);
    for (Node* child : this->getChildren())
    {
        if (child == this->renderTexture) continue;
        child->visit();
    }
    this->renderTexture->end();
}

void FocusLightLayer::visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    if (!this->isVisible()) return;
    
    this->renderTexture->getSprite()->visit(renderer, parentTransform, parentFlags);
}
