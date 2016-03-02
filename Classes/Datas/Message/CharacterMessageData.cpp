//
//  CharacterMessageData.cpp
//  LastSupper
//
//  Created by Kohei on 2015/10/12.
//
//

#include "Datas/Message/CharacterMessageData.h"

// create関数
CharacterMessageData* CharacterMessageData::createWithCharaId(int charaId, const queue<string>& pages, int imgId, Option option)
{
    CharacterMessageData* p { CharacterMessageData::create(pages)};
    if(!p) return nullptr;
    p->charaId = charaId;
    p->imgId = imgId;
    p->option = option;
    
    return p;
}

// create関数
CharacterMessageData* CharacterMessageData::createWithCharaName(const string& charaName, const queue<string>& pages, int charaId, int imgId, Option option)
{
    CharacterMessageData* p { CharacterMessageData::create(pages)};
    if(!p) return nullptr;
    p->charaName = charaName;
    p->charaId = charaId;
    p->imgId = imgId;
    p->option = option;
    
    return p;
}

// コンストラクタ
CharacterMessageData::CharacterMessageData():charaName("") {FUNCLOG};

// デストラクタ
CharacterMessageData::~CharacterMessageData() {FUNCLOG};

// 初期化
bool CharacterMessageData::init(const queue<string>& pages)
{
    this->pages = pages;
    
    return true;
}

// 次のページがあるか
bool CharacterMessageData::hasNextPage()
{
    return !this->pages.empty();
}

// メッセージを取得
const string CharacterMessageData::getMessage()
{
    string message {this->pages.front()};
    this->pages.pop();
    
    return message;
}

// キャラIDを取得
int CharacterMessageData::getCharaId() const
{
    return this->charaId;
}

// 画像IDを取得
int CharacterMessageData::getImgId() const
{
    return this->imgId;
}

// キャラクター名を取得
const string& CharacterMessageData::getCharaName() const
{
    return this->charaName;
}

// オプションを取得
CharacterMessageData::Option CharacterMessageData::getOption() const
{
    return this->option;
}

void CharacterMessageData::setCharaId(int charaId)
{
    this->charaId = charaId;
}
void CharacterMessageData::setImgId(int imgId)
{
    this->imgId = imgId;
}
void CharacterMessageData::setCharaName(const string& charaName)
{
    this->charaName = charaName;
}
void CharacterMessageData::setOption(Option option)
{
    this->option = option;
}