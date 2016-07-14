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
    
    string title = "InfoAssert";
    AssertScene* assert { AssertScene::create(title, message, true) };
    
    // assertをプッシュ
    Director::getInstance()->pushScene(assert);
}

void LastSupper::AssertUtils::fatalAssert(const string& message)
{
    string title = DebugManager::getInstance()->isPlainData() ? "FatalAssert" : "SystemError";

    AssertScene* assert { AssertScene::create(title, message, false) };
    
    // assertをプッシュ
    Director::getInstance()->pushScene(assert);
}
