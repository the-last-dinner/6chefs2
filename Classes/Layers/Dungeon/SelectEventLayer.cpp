//
//  SelectEventLayer.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/15.
//
//

#include "Layers/Dungeon/SelectEventLayer.h"

#include "Datas/Message/SystemMessageData.h"
#include "Datas/Message/CharacterMessageData.h"

#include "Layers/Message/SystemMessageLayer.h"
#include "Layers/Message/CharacterMessageLayer.h"
#include "Layers/Menu/MiniSelector.h"


// コンストラクタ
SelectEventLayer::SelectEventLayer() {FUNCLOG};

// デストラクタ
SelectEventLayer::~SelectEventLayer() {FUNCLOG};

// 初期化
bool SelectEventLayer::init(const string& message, const vector<string>& choices, const queue<CharacterMessageData*>& datas)
{
    if(!Layer::init()) return false;
    
    this->setCascadeOpacityEnabled(true);
    SpriteUtils::Square position = SpriteUtils::Square(75, 20, 95, 40); // 位置
    // datasが空ならシステムメッセージで質問文表示
    if(datas.empty())
    {
        SystemMessageLayer* messageLayer { SystemMessageLayer::create(SystemMessageData::create(message), nullptr) };
        this->addChild(messageLayer);
    }
    else
    {
        // 空でない時はキャラメッセージ
        CharacterMessageLayer* charaMessageLayer { CharacterMessageLayer::create(datas, nullptr) };
        this->addChild(charaMessageLayer);
        position = SpriteUtils::Square(77, 33, 97, 53); // 位置修正
    }
    
    // 選択レイヤ表示
    Point index = Point(1,choices.size()); // 要素数
    //Size parcent = Size(WINDOW_WIDTH/100, WINDOW_HEIGHT/100);
    //Sprite* window {Sprite::createWithSpriteFrameName("question_selector.png")};
    //window->setPosition(window->getContentSize().width/2 + parcent.width * 75, window->getContentSize().height/2 + parcent.height * 20);
    //MiniSelector::SelectorWithSprite selector = MiniSelector::SelectorWithSprite(index, window, choices);
    MiniSelector::Selector selector = MiniSelector::Selector(index, position, choices);
    MiniSelector* mini = {MiniSelector::create(selector)};
    mini->show();
    mini->onMiniIndexSelected = CC_CALLBACK_1(SelectEventLayer::onChoiceSelected, this);
    
    this->addChild(mini);
    this->mini = mini;
    
    return true;
}

// 選択肢が選ばれた時
void SelectEventLayer::onChoiceSelected(const int idx)
{
    // MIniSelectorのリスナを無効に
    this->mini->hide();
    
    if(this->onSelected)
    {
        this->onSelected(idx);
    }
    
    this->runAction(Sequence::createWithTwoActions(FadeOut::create(0.5f), RemoveSelf::create()));
}
