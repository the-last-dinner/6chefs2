//
//  ShaderLayer.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/19.
//
//

#ifndef _ShaderLayer__
#define _ShaderLayer__

#include "Common.h"

class ShaderLayer : public Layer
{
// 定数
public:
    
// インスタンス変数
protected:
    GLProgram* glProgram { nullptr };
    GLProgramState* glProgramState { nullptr };
    RenderTexture* renderTexture { nullptr };
    Sprite* renderTexSprite { nullptr };
 
// インスタンスメソッド
public:
    ShaderLayer();   // コンストラクタ
    ~ShaderLayer();  // デストラクタ
    virtual bool init(const string& vertFilePath, const string& fragFilePath);    // 初期化
    virtual void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags);
};

#endif /* defined(_ShaderLayer__) */
