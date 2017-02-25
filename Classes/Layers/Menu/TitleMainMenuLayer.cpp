//
//  TitleMainMenuLayer.cpp
//  LastSupper
//
//  Created by Kohei on 2015/07/05.
//
//

#include "Layers/Menu/TitleMainMenuLayer.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

#include "UI/Cloud.h"
#include "UI/NotificationBand.h"

// コンストラクタ
TitleMainMenuLayer::TitleMainMenuLayer(){FUNCLOG}

// デストラクタ
TitleMainMenuLayer::~TitleMainMenuLayer(){FUNCLOG}

// 初期化
bool TitleMainMenuLayer::init()
{
    map<MenuType, string> typeToString
    {
        {MenuType::START, "はじめから"},
        {MenuType::CONTINUE, "つづきから"},
        {MenuType::TROPHY, "トロフィ"},
    };
    this->specialRoomTitle = PlayerDataManager::getInstance()->getGlobalData()->isCleared() ? "おまけ" : "? ? ?";
    if (ConfigDataManager::getInstance()->getMasterConfigData()->isDisplay(MasterConfigData::SPECIAL_ROOM))
    {
        typeToString.insert({MenuType::SPECIAL_ROOM, this->specialRoomTitle});
    }
    else
    {
        typeToString.insert({MenuType::EXIT, "終了"});
    }
    
	if(!MenuLayer::init(1, typeToString.size())) return false;
    
    MasterConfigData* masterConfigData {ConfigDataManager::getInstance()->getMasterConfigData()};
    
    this->animating = true;
	
	// 背景画像を生成
	Sprite* titleBg = Sprite::createWithSpriteFrameName("background.png");
	titleBg->setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	this->addChild(titleBg);
    
    // タイトル文字を生成
    float font_size = 56.f;
    
    Label* title1 {Label::createWithTTF("6人の料理人", Resource::Font::SYSTEM, font_size)};
    title1->setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - title1->getContentSize().height * 1.6);
    title1->setColor(Color3B::Color3B(200,0,0));
    title1->setOpacity(0);
    this->addChild(title1);
    
    Label* title2 {Label::createWithTTF("と", Resource::Font::SYSTEM, font_size)};
    title2->setPosition(WINDOW_WIDTH/2, title1->getPosition().y - title2->getContentSize().height);
    title2->setOpacity(0);
    title2->setColor(Color3B::WHITE);
    this->addChild(title2);
    
    Label* title3 {Label::createWithTTF("隻眼の少女", Resource::Font::SYSTEM, font_size)};
    title3->setPosition(WINDOW_WIDTH/2, title2->getPosition().y - title3->getContentSize().height);
    title3->setColor(Color3B::Color3B(200,0,0));
    title3->setOpacity(0);
    this->addChild(title3);
    
    // 2のアイコン
    Spawn* titleNumberAction { nullptr };
    if (masterConfigData->isDisplay(MasterConfigData::TWO_ICON)) {
        float titleNumberScale { 0.33f };
        Sprite* titleNumber {Sprite::createWithSpriteFrameName("title_2.png")};
        titleNumber->setPosition(
            WINDOW_WIDTH/2 + title3->getContentSize().width/2 + titleNumber->getContentSize().width * titleNumberScale / 5,
            title2->getPosition().y - title3->getContentSize().height
        );
        titleNumber->setOpacity(0);
        this->addChild(titleNumber);
        titleNumberAction = Spawn::createWithTwoActions(
                                TargetedAction::create(titleNumber, FadeIn::create(0.8f)),
                                TargetedAction::create(titleNumber, EaseCubicActionOut::create(ScaleTo::create(0.6f, titleNumberScale)))
                            );
    }
    
    // タイトルメニューを生成
	float menuSize = 44.f;
    float duration { 1.0f };
    float latency { 0.2f };
	for(int i = 0; i < etoi(MenuType::SIZE); i++)
	{
        Label* menuItem { Label::createWithTTF(typeToString[static_cast<MenuType>(i)], Resource::Font::SYSTEM, menuSize) };
		menuItem->setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT * 0.525 - (menuSize + 12) * i);
		menuItem->setOpacity(0);
		this->addChild(menuItem);
		this->menuObjects.push_back(menuItem);
		
		menuItem->runAction(Sequence::create(DelayTime::create(latency * i),
                                             Spawn::create(MoveBy::create(duration, Vec2(0, -20)), FadeIn::create(duration), nullptr),
                                             nullptr));
	}
    
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(duration + latency * etoi(MenuType::SIZE)), CallFunc::create(CC_CALLBACK_0(TitleMainMenuLayer::onEnterAnimationFinished, this))));
	
    this->runAction(Sequence::create(
        TargetedAction::create(title1, FadeIn::create(0.8f)),
        TargetedAction::create(title2, FadeTo::create(0.8f, 200)),
        TargetedAction::create(title3, FadeIn::create(0.8f)),
        titleNumberAction,
        nullptr
    ));
    
    // copyright
    Label* copyright {Label::createWithTTF(masterConfigData->getString(MasterConfigData::COPYRIGHT), Resource::Font::MESSAGE, 16)};
    copyright->setPosition(Point(WINDOW_WIDTH - copyright->getContentSize().width * 0.52f, copyright->getContentSize().height));
    copyright->setOpacity(0);
    this->addChild(copyright);
    copyright->runAction(FadeTo::create(1.f, 200));
    
    // 操作方法
    Label* opr2 {Label::createWithTTF("F1:操作設定 / F4:画面サイズ設定", Resource::Font::MESSAGE, 18)};
    opr2->setPosition(WINDOW_WIDTH / 2, opr2->getContentSize().height / 2 + copyright->getContentSize().height * 2);
    opr2->setColor(Color3B::WHITE);
    opr2->setOpacity(0);
    this->addChild(opr2);
    opr2->runAction(FadeTo::create(1.2f, 200));
    
    Label* opr {Label::createWithTTF("Enter:決定 / X:戻る", Resource::Font::MESSAGE, 18)};
    opr->setPosition(WINDOW_WIDTH / 2, opr->getContentSize().height + opr2->getContentSize().height + copyright->getContentSize().height * 2);
    opr->setColor(Color3B::WHITE);
    opr->setOpacity(0);
    this->addChild(opr);
    opr->runAction(FadeTo::create(1.2f, 200));
    
    // バージョン表記
    Label* version { Label::createWithTTF(masterConfigData->getString(MasterConfigData::VERSION), Resource::Font::MESSAGE, 18) };
    version->setPosition(Point(version->getContentSize().width/2, version->getContentSize().height));
    version->setColor(Color3B::WHITE);
    version->setOpacity(0);
    this->addChild(version);
    version->runAction(FadeTo::create(1.2f, 200));
    
	return true;
}

// タイトルメニュー表示のためのアニメーションが終了した時
void TitleMainMenuLayer::onEnterAnimationFinished()
{
    // カーソル生成
    Cloud* cursor { Cloud::create(Size::ZERO) };
    cursor->setColor(Color3B(100, 0, 0));
    cursor->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
    this->addChild(cursor);
    this->cursor = cursor;
    
    // カーソルを初期位置まで移動
    this->onIndexChanged(1, false);
    this->setSelectedIndex(1);
    
    // リスナを有効化
    this->listenerKeyboard->setEnabled(true);
    
    this->animating = false;
}

// 表示
void TitleMainMenuLayer::show()
{
	if(!this->animating) this->listenerKeyboard->setEnabled(true);
}

// 非表示
void TitleMainMenuLayer::hide()
{
	this->listenerKeyboard->setEnabled(false);
}

// 選択しているindexが変わった時
void TitleMainMenuLayer::onIndexChanged(int newIdx, bool sound)
{
	for(int i = 0; i < MenuLayer::menuObjects.size(); i++)
	{
        if (newIdx != i) continue;
        
        Node* obj {this->menuObjects.at(i)};
        this->cursor->setVisible(true);
        this->cursor->setScale((obj->getContentSize().width + 50) / this->cursor->getContentSize().width);
        this->cursor->setPosition(obj->getPosition());
	}
    if(sound)SoundManager::getInstance()->playSE(Resource::SE::CURSOR_MOVE);
	return;
}

// 決定キーを押した時
void TitleMainMenuLayer::onEnterKeyPressed(int idx)
{
	map<MenuType, function<void()>> typeMap
	{
		{MenuType::START, this->onStartSelected},
		{MenuType::CONTINUE, this->onContinueSelected},
        {MenuType::TROPHY, this->onTrophySelected},
        {MenuType::SPECIAL_ROOM, this->onSpecialRoomSelected},
	};
    if (ConfigDataManager::getInstance()->getMasterConfigData()->isDisplay(MasterConfigData::SPECIAL_ROOM))
    {
    }
    MenuType menu {static_cast<MenuType>(idx)};
	if(!typeMap.count(menu)) return;
    
    // クリアしていない場合
    if (!PlayerDataManager::getInstance()->getGlobalData()->isCleared())
    {
        // トロフィー見れない
        if (menu == MenuType::TROPHY)
        {
            this->prohibitNotification("トロフィーはクリア後に見ることができます");
            return;
        }
        
        // おまけ部屋を見れない
        if (menu == MenuType::SPECIAL_ROOM) {
            this->prohibitNotification(this->specialRoomTitle + "はクリア後に見ることができます");
            return;
        }
    }
    
    
    // 選択されたメニューに応じてコールバック関数実行
	if(function<void()> callback {typeMap.at(menu)}) callback();
}

// トロフィーを見れない通知
void TitleMainMenuLayer::prohibitNotification(const string& msg)
{
    if (notification)
    {
        SoundManager::getInstance()->playSE(Resource::SE::BACK);
        this->notification->hide([this]{
            this->removeChild(this->notification);
            this->notification = nullptr;
            this->setCursorEnable(true);
        });
    }
    else
    {
        this->setCursorEnable(false);
        SoundManager::getInstance()->playSE(Resource::SE::FAILURE);
        NotificationBand* notification = NotificationBand::create(msg);
        notification->setBandColor(Color3B(64,0,0));
        this->addChild(notification);
        notification->show();
        this->notification = notification;
    }
}
