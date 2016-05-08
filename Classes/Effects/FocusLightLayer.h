//
//  FocusLightLayer.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/08.
//
//

#ifndef FocusLightLayer_h
#define FocusLightLayer_h

#include "define.h"

class FocusLightLayer : public Layer
{
// 定数
private:
    static const Color3B AMBIENT_LIGHT_COLOR;
    
// クラスメソッド
public:
    CREATE_FUNC(FocusLightLayer)
    
// インスタンス変数
private:
    Sprite* ambientSprite { nullptr };
    map<Node*, Sprite*> lightMap {};
    RenderTexture* renderTexture { nullptr };

// インスタンスメソッド
private:
    FocusLightLayer();
    virtual ~FocusLightLayer();
    virtual bool init() override;
    void onEnter() override;
    void onExit() override;
    void removeFocusLight(Node* target);
public:
    void addTarget(Node* target);
    void update(float delta) override;
    virtual void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) override;
};

#endif /* FocusLightLayer_h */
