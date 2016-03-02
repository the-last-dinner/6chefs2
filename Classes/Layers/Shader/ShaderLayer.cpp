//
//  ShaderLayer.cpp
//  LastSupper
//
//  Created by Kohei on 2015/09/19.
//
//

#include "Layers/Shader/ShaderLayer.h"

// コンストラクタ
ShaderLayer::ShaderLayer()
{ FUNCLOG }

// デストラクタ
ShaderLayer::~ShaderLayer()
{ FUNCLOG }

// 初期化
bool ShaderLayer::init(const string& vertFilePath, const string& fragFilePath)
{
    if(!Layer::init()) return false;
    
    // シェーダープログラム生成
    GLProgram* glProgram { GLProgram::createWithFilenames(vertFilePath, fragFilePath) };
    glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION,GLProgram::VERTEX_ATTRIB_POSITION);
    glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
    glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
    glProgram->link();
    glProgram->updateUniforms();
    this->glProgram = glProgram;
    
    // シェーダーステート生成
    GLProgramState* glProgramState { GLProgramState::getOrCreateWithGLProgram(glProgram) };
    this->glProgramState = glProgramState;
    
    // 画面サイズ取得
    Size winSize { Director::getInstance()->getWinSize() };
    
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
    
    renderTexSprite->setGLProgram(glProgram);
    
    return true;
}

void ShaderLayer::visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    this->renderTexture->beginWithClear(0, 0, 0, 0);
    for (auto child : this->getChildren())
    {
        if (child != this->renderTexture && child != this->renderTexSprite)
            child->visit(renderer, parentTransform, parentFlags);
    }
    this->renderTexture->end();
    
    this->renderTexSprite->visit(renderer, parentTransform, parentFlags);
}