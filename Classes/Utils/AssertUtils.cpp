//
//  AssertUtils.cpp
//  6chefs2
//
//  Created by S.inami on 2016/07/13.
//
//

#include "AssertUtils.h"

#include "Scenes/AssertScene.h"

void LastSupper::AssertUtils::infoAssert(const string& message)
{
    // デバッグじゃない時は出さない
    if(!ConfigDataManager::getInstance()->getDebugConfigData()->isDebugMode()) return;
    
    AssertScene* assert { AssertScene::create(message, AssertScene::AssertType::INFO) };
    
    BaseScene* nowScene = (BaseScene*)Director::getInstance()->getRunningScene();
    assert->onEnterPushedScene = CC_CALLBACK_0(BaseScene::onEnterPushedScene, nowScene);
    assert->onExitPushedScene = CC_CALLBACK_0(BaseScene::onExitPushedScene, nowScene);
    Director::getInstance()->pushScene(assert);
}

void LastSupper::AssertUtils::warningAssert(const string& message)
{
    // デバッグじゃない時は出さない
    if(!ConfigDataManager::getInstance()->getDebugConfigData()->isDebugMode()) return;
    
    AssertScene* assert { AssertScene::create(message, AssertScene::AssertType::WARNING) };
    
    BaseScene* nowScene = (BaseScene*)Director::getInstance()->getRunningScene();
    assert->onEnterPushedScene = CC_CALLBACK_0(BaseScene::onEnterPushedScene, nowScene);
    assert->onExitPushedScene = CC_CALLBACK_0(BaseScene::onExitPushedScene, nowScene);
    Director::getInstance()->pushScene(assert);}

void LastSupper::AssertUtils::fatalAssert(const string& message)
{
    AssertScene::AssertType assertType = ConfigDataManager::getInstance()->getDebugConfigData()->isDebugMode() ? AssertScene::AssertType::FATAL : AssertScene::AssertType::ERROR;
    AssertScene* assert { AssertScene::create(message, assertType) };
    
    BaseScene* nowScene = (BaseScene*)Director::getInstance()->getRunningScene();
    assert->onEnterPushedScene = CC_CALLBACK_0(BaseScene::onEnterPushedScene, nowScene);
    assert->onExitPushedScene = CC_CALLBACK_0(BaseScene::onExitPushedScene, nowScene);
    Director::getInstance()->pushScene(assert);
}