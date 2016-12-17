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
    
    // Selectorの大きさ設定
    int choiceCount = (int) choices.size();
    int maxY = 22 + (choiceCount * 8);
    int minY = 20;
    int maxStrLen {9};
    int temp {0};
    for (auto choice : choices)
    {
        temp = (int) choice.size();
        if (maxStrLen < temp) maxStrLen = temp;
    }
    
    
    if(datas.empty()) {
        // datasが空ならシステムメッセージで質問文表示
        SystemMessageLayer* messageLayer { SystemMessageLayer::create(SystemMessageData::create(message), nullptr) };
        this->addChild(messageLayer);
    } else {
        // 空でない時はキャラメッセージ
        CharacterMessageLayer* charaMessageLayer { CharacterMessageLayer::create(datas, nullptr) };
        this->addChild(charaMessageLayer);
        // 高さ調整
        minY += 13;
        maxY += 10;
    }
    
    
    // 選択レイヤ表示
    Point index = Point(1,choices.size()); // 要素数
    SpriteUtils::Square position { SpriteUtils::Square(88 - maxStrLen, minY, 95, maxY) };
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
