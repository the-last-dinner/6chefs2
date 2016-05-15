//
//  Transition.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/14.
//
//

#ifndef Transition_h
#define Transition_h

#include "define.h"

class AbstractTransition : public Scene
{
// インスタンス変数
public:
    Scene* _inScene { nullptr };
    Scene* _outScene { nullptr };
    function<void(AbstractTransition*)> onTransitionFinished { nullptr };
    
// インスタンスメソッド
protected:
    AbstractTransition() {};
    virtual ~AbstractTransition() {};
    virtual bool init(Scene* scene);
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void draw(Renderer* renderer, const Mat4& transform, u_int32_t flags) override;
};


namespace Transition
{
    class Fade : public AbstractTransition
    {
    // クラスメソッド
    public:
        CREATE_FUNC_WITH_TWO_PARAM(Fade, Scene*, float)
        
    // インスタンス変数
    private:
        float duration {};
    
    // インスタンスメソッド
    private:
        Fade() { FUNCLOG };
        ~Fade() { FUNCLOG };
        virtual bool init(Scene* scene, float duration);
    };
}

#endif /* Transition_h */
