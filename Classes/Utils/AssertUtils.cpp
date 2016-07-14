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
    if(!DebugManager::getInstance()->isPlainData()) return;
    
    // Assert画面のタイトル
    string title = "InfoAssert";
    
    BaseScene* nowScene = (BaseScene*)Director::getInstance()->getRunningScene();
    AssertScene* assert { AssertScene::create(title, message, true) };
    assert->onEnterAssertScene = CC_CALLBACK_0(BaseScene::onEnterAssertScene, nowScene);
    assert->onExitAssertScene = CC_CALLBACK_0(BaseScene::onExitAssertScene, nowScene);
    // assertをプッシュ
    Director::getInstance()->pushScene(assert);
}

void LastSupper::AssertUtils::fatalAssert(const string& message)
{
    // Assert画面のタイトル
    string title = DebugManager::getInstance()->isPlainData() ? "FatalAssert" : "SystemError";
    
    // 戻ってこないのでpushのみ実行
    BaseScene* nowScene = (BaseScene*)Director::getInstance()->getRunningScene();
    AssertScene* assert { AssertScene::create(title, message, false) };
    assert->onEnterAssertScene = CC_CALLBACK_0(BaseScene::onEnterAssertScene, nowScene);

    // assertをプッシュ
    Director::getInstance()->pushScene(assert);
}
