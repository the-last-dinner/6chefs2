//
//  StoryMessageData.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/12.
//
//

#include "Datas/Message/StoryMessageData.h"

// コンストラクタ
StoryMessageData::StoryMessageData() {FUNCLOG};

// デストラクタ
StoryMessageData::~StoryMessageData() {FUNCLOG};

// 初期化
bool StoryMessageData::init(const string& message)
{
    this->message = message;
    
    return true;
}

// メッセージを取得
const string StoryMessageData::getMessage()
{
    return this->message;
}