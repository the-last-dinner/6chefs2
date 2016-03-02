//
//  SystemMessageData.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/12.
//
//

#include "Datas/Message/SystemMessageData.h"

// コンストラクタ
SystemMessageData::SystemMessageData() {FUNCLOG};

// デストラクタ
SystemMessageData::~SystemMessageData() {FUNCLOG};

// 初期化
bool SystemMessageData::init(const string& message)
{
    this->message = message;
    
    return true;
}

// メッセージを取得
const string SystemMessageData::getMessage()
{
    return this->message;
}