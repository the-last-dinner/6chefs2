//
//  Transition.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/14.
//
//

#include "Transition/Transition.h"

#include "Managers/SceneManager.h"

# pragma mark AbstractTransition

bool AbstractTransition::init(Scene* scene)
{
    if(!Scene::init()) return false;
    
    this->_inScene = scene;
    this->_inScene->retain();
    this->_outScene = Director::getInstance()->getRunningScene();
    if (!this->_outScene)
    {
        this->_outScene = Scene::create();
    }
    this->_outScene->retain();
    
    CCASSERT( this->_inScene != this->_outScene, "Incoming scene must be different from the outgoing scene" );
    
    return true;
}

void AbstractTransition::onEnter()
{
    Scene::onEnter();
    
    this->_outScene->onExitTransitionDidStart();
    this->_inScene->onEnter();
}

void AbstractTransition::onExit()
{
    Scene::onExit();
    
    this->_outScene->onExit();
    this->_inScene->onEnterTransitionDidFinish();
}

void AbstractTransition::draw(Renderer* renderer, const Mat4& transform, u_int32_t flags)
{
    Scene::draw(renderer, transform, flags);
    this->_outScene->visit(renderer, transform, flags);
    this->_inScene->visit(renderer, transform, flags);
}

# pragma mark -
# pragma mark Fade

bool Transition::Fade::init(Scene* scene, float duration)
{
    if(!AbstractTransition::init(scene)) return false;
    
    this->duration = duration;
    
    return true;
}
