//
//  SceneManager.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/14.
//
//

#include "Managers/SceneManager.h"

#include "Scenes/RootScene.h"

// 唯一のインスタンス
static SceneManager* _instance { nullptr };

# pragma mark static

SceneManager* SceneManager::getInstance()
{
    if(!_instance) _instance = new SceneManager();
    
    return _instance;
}

void SceneManager::destroy()
{
    delete _instance;
}

# pragma mark -
# pragma mark private

// コンストラクタ
SceneManager::SceneManager() { FUNCLOG };

// デストラクタ
SceneManager::~SceneManager() { FUNCLOG };

# pragma mark -
# pragma mark getter

RootScene* SceneManager::getRootScene() const
{
    return dynamic_cast<RootScene*>(Director::getInstance()->getRunningScene());
}

Scene* SceneManager::getRunningScene() const
{
    return this->_runningScene;
}

# pragma mark -
# pragma mark public

void SceneManager::runWithScene(Scene* scene)
{
    CCASSERT(scene != nullptr, "This command can only be used to start the SceneManager. There is already a scene present.");
    CCASSERT(this->_runningScene == nullptr, "_runningScene should be null");
    
    this->pushScene(scene);
}

void SceneManager::replaceScene(Scene* scene)
{
    CCASSERT(scene != nullptr, "the scene should not be null");
    
    if(!this->_runningScene)
    {
        runWithScene(scene);
        return;
    }
    
    if(scene == this->_runningScene) return;
    
    ssize_t index { this->_sceneStack.size() };
    this->_sceneStack.replace(index - 1, scene);
    
    this->setNextScene(scene);
}

void SceneManager::pushScene(Scene* scene)
{
    CCASSERT(scene, "the scene should not null");
    
    this->_sceneStack.pushBack(scene);
    
    this->setNextScene(scene);
}

void SceneManager::popScene()
{
    CCASSERT(this->_runningScene != nullptr, "running scene should not null");
    
    this->_sceneStack.popBack();
    ssize_t c = this->_sceneStack.size();
    
    if (c == 0)
    {
        this->destroy();
    }
    else
    {
        this->setNextScene(this->_sceneStack.at(c - 1));
    }
}

void SceneManager::setNextScene(Scene* scene)
{
    if(this->_runningScene)
    {
        this->_runningScene->removeFromParent();
    }
    
    this->_runningScene = scene;
    
    this->getRootScene()->addChild(scene);
}
