//
//  PasswordEventLayer.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/29.
//
//

#include "Layers/Dungeon/PasswordEventLayer.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "Layers/Menu/NumberSelector.h"

// create関数
PasswordEventLayer* PasswordEventLayer::create(const string& password, ResultCallback callback)
{
    PasswordEventLayer* p { new(nothrow) PasswordEventLayer() };
    if(p && p->init(password, callback))
    {
        CC_SAFE_RETAIN(p);
        return p;
    }
    else
    {
        delete p;
        p = nullptr;
        return nullptr;
    }
}

// コンストラクタ
PasswordEventLayer::PasswordEventLayer(){FUNCLOG}

// デストラクタ
PasswordEventLayer::~PasswordEventLayer(){FUNCLOG}

// 初期化
bool PasswordEventLayer::init(const string& password, ResultCallback callback)
{
    this->pass_length = password.length();
    if(!Layer::init() || this->pass_length < 1 ||  !callback) return false;
    
    // 変数の保存
    this->password = password;
    this->callback = callback;
    this->setOpacity(0);
    this->setScale(0);
    this->setCascadeOpacityEnabled(true);
    
    // 背景を薄暗く設定
    Sprite* back {Sprite::create()};
    back->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    back->setPosition(back->getContentSize().width/2, back->getContentSize().height/2);
    back->setOpacity(64);
    this->addChild(back);
    
    // 入力された数字パネル
    SpriteUtils::Square square = SpriteUtils::Square(30,60,70,80);
    SpriteUtils::Margin margin = SpriteUtils::Margin(3);
    Sprite* numPanel = SpriteUtils::getSquareSprite(square, margin);
    numPanel->setOpacity(128);
    back->addChild(numPanel);
    
    // 数字ラベル
    Size numPanelSize = numPanel->getContentSize();
    float eachSize = numPanelSize.width / this->pass_length;
    for (int i = 0; i < this->pass_length; i++)
    {
        Label* label { Label::createWithTTF("-", "fonts/cinecaption2.28.ttf", 48) };
        label->setPosition(Point(i * eachSize + eachSize/2, numPanelSize.height / 2));
        label->setCascadeOpacityEnabled(true);
        numPanel->addChild(label);
        this->confirmLabels.push_back(label);
    }
    
    // ナンバーセレクターの表示
    square = SpriteUtils::Square(37.5,10,62.5,50);
    NumberSelector* numSelector {NumberSelector::create(square)};
    numSelector->onNumberSelectorSelected = CC_CALLBACK_1(PasswordEventLayer::onNumberSelected, this);
    numSelector->onNumberSelectorCanceled= CC_CALLBACK_0(PasswordEventLayer::onNumberSelectCanceled, this);
    numSelector->show();
    back->addChild(numSelector);
    
    //生成
    this->show();
    
    return true;
}

// 数字が選ばれた時
void PasswordEventLayer::onNumberSelected(int num)
{
    // チェック状態確認
    if(this->checking) return;
    
    // ボタンにおける処理
    int index = this->inputed.size();
    if (num<10)
    {
        // 入力登録
        this->inputed.push_back(to_string(num));
        this->confirmLabels[index]->setString(to_string(num));
    }
    else
    {
        // 入力削除
        if(index == 0) return;
        this->inputed.pop_back();
        this->confirmLabels[index-1]->setString("-");
    }
    
    
    // サイズの確認
    int len = this->inputed.size();
    if (len >= this->pass_length)
    {
        string inputed_string = "";
        for(int i = 0; i < len; i++)
        {
            inputed_string += this->inputed[i];
        }
        
        // 合致しているかチェック
        Result result = inputed_string == this->password ? Result::SUCCESS : Result::FAILURE;
        this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([this]{this->hide();}), DelayTime::create(0.2f), CallFunc::create([this]{this->hide();}), CallFunc::create([this,result]{this->callback(result);}), RemoveSelf::create(), nullptr));
        this->checking = true;
    }
}

// キャンセル時
void PasswordEventLayer::onNumberSelectCanceled()
{
    this->runAction(Sequence::create(CallFunc::create([this]{this->hide();}), DelayTime::create(0.2f), CallFunc::create([this]{this->callback(Result::FAILURE);}), RemoveSelf::create(), nullptr));
}

void PasswordEventLayer::show()
{
    this->runAction(ScaleTo::create(0.2f, 1.f));
}

void PasswordEventLayer::hide()
{
    this->runAction(ScaleTo::create(0.2f, 0));
}
