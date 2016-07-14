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
    AssertScene* assert { AssertScene::create(message, true) };
    
    // assertをプッシュ
    Director::getInstance()->pushScene(assert);
}

void LastSupper::AssertUtils::fatalAssert(const string& message)
{
    AssertScene* assert { AssertScene::create(message, false) };
    
    // assertをプッシュ
    Director::getInstance()->pushScene(assert);
}
