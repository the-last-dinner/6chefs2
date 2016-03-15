//
//  ItemMenuLayer.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/01.
//
//

#include "Layers/Menu/ItemMenuLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "Scenes/DungeonScene.h"
#include "Layers/Menu/MiniSelector.h"
#include "UI/NotificationBand.h"

// コンストラクタ
ItemMenuLayer::ItemMenuLayer(){FUNCLOG}

// デストラクタ
ItemMenuLayer::~ItemMenuLayer(){FUNCLOG}

// ItemMenuLayer生成
bool ItemMenuLayer::init()
{
    FUNCLOG
    // メニュー設定
    int obj_count = PlayerDataManager::getInstance()->getLocalData()->getItemAll().size();
    Point maxSize = Point(3,6);
    int sizeX = obj_count < maxSize.x ? obj_count : maxSize.x;
    int sizeY = obj_count < maxSize.x * maxSize.y ? floor((obj_count - 1 )/ maxSize.x) + 1 : maxSize.y;
    Size size = Size(sizeX, sizeY);
    int page_size = floor(abs(obj_count-1 ) / (maxSize.x * maxSize.y)) + 1;
    if (!MenuLayer::init(size, page_size)) return false;
    
    SpriteUtils::Square square;
    SpriteUtils::Margin margin;
    Size parcent = Size(WINDOW_WIDTH/100, WINDOW_HEIGHT/100);
    
    // 白い背景を生成
    Sprite* white = Sprite::create();
    white->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    white->setColor(Color3B::WHITE);
    white->setPosition(WINDOW_CENTER);
    //white->setOpacity(128);
    this->addChild(white);
    
    // タイトル
    //square = SpriteUtils::Square(0,80,30,100);
    //margin = SpriteUtils::Margin(3.0,1.5,1.5,3.0);
    //Sprite* leftTop = SpriteUtils::getSquareSprite(square, margin);
    //leftTop->setColor(Color3B(128,0,0));
    Sprite* leftTop {Sprite::createWithSpriteFrameName("menu_title_panel.png")};
    leftTop->setPosition(leftTop->getContentSize().width/2, WINDOW_HEIGHT - leftTop->getContentSize().height/2);
    this->addChild(leftTop);
    
    Label* title = Label::createWithTTF("アイテム", "fonts/cinecaption2.28.ttf", 48);
    title->setPosition(leftTop->getContentSize().width / 2, leftTop->getContentSize().height / 2);
    title->setColor(Color3B::WHITE);
    leftTop->addChild(title);
    
    // アイテム詳細
    //square = SpriteUtils::Square(0,0,100,25);
    //margin = SpriteUtils::Margin(1.5,3.0,3.0,3.0);
    //Sprite* bottom = SpriteUtils::getSquareSprite(square, margin);
    //bottom->setColor(Color3B::BLACK);
    Sprite* bottom {Sprite::createWithSpriteFrameName("item_detail.png")};
    bottom->setPosition(bottom->getContentSize().width/2, bottom->getContentSize().height/2);
    bottom->setName("bottom");
    this->addChild(bottom);
    
    // 装備
    //square = SpriteUtils::Square(30,80,100,100);
    //margin = SpriteUtils::Margin(3.0,3.0,1.5,1.5);
    //Sprite* rightTop = SpriteUtils::getSquareSprite(square, margin);
    //rightTop->setColor(Color3B::BLACK);
    Sprite* rightTop {Sprite::createWithSpriteFrameName("item_equipment.png")};
    rightTop->setPosition(rightTop->getContentSize().width/2 + parcent.width * 30, rightTop->getContentSize().height/2 + parcent.height * 80);
    this->addChild(rightTop);
    
    int right_id = PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(Direction::RIGHT);
    int left_id = PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(Direction::LEFT);
    string right_equip = (right_id != 0) ? CsvDataManager::getInstance()->getItemData()->getItemName(right_id) : "なし";
    string left_equip = (left_id != 0) ? CsvDataManager::getInstance()->getItemData()->getItemName(left_id) : "なし";
    Label* equip_title = Label::createWithTTF("装備", "fonts/cinecaption2.28.ttf", 26);
    equip_title->setPosition(equip_title->getContentSize().width/2 + 30, rightTop->getContentSize().height - equip_title->getContentSize().height/2 - 15);
    equip_title->setColor(Color3B::WHITE);
    rightTop->addChild(equip_title);
    Label* equipment = Label::createWithTTF("右手 : " + right_equip + "\n左手 : " + left_equip, "fonts/cinecaption2.28.ttf", 26);
    equipment->setPosition(equipment->getContentSize().width/2 + 20, equipment->getContentSize().height/2 + 20);
    equipment->setColor(Color3B::WHITE);
    rightTop->addChild(equipment);
    
    // アイテムリスト
    square = SpriteUtils::Square(0,25,100,80);
    //margin = SpriteUtils::Margin(1.5,3.0,1.5,3.0);
    margin = SpriteUtils::Margin(1.0);
    Sprite* center = SpriteUtils::getSquareSprite(square, margin);
    center->setColor(Color3B::BLACK);
    //Sprite* center {Sprite::createWithSpriteFrameName("item_selector.png")};
    //center->setPosition(center->getContentSize().width/2, center->getContentSize().height/2 + parcent.height * 25);
    center->setName("itemList");
    CC_SAFE_RETAIN(center);
    this->addChild(center);
    
    vector<int> items = PlayerDataManager::getInstance()->getLocalData()->getItemAll();
    if (items.empty()){
        // アイテムが時は空用の番号をインサート
        items.push_back(-1);
    }
    int i = 0;
    int page = 0;
    int upDownMargin = 40;
    Size centerSize {center->getContentSize()};
    for(auto itr : items)
    {
        // ページパネル生成
        page = floor(i / (maxSize.x * maxSize.y));
        if (this->pagePanels.size() < page + 1)
        {
            Sprite* page_panel = Sprite::create();
            page_panel->setTextureRect(Rect(0,0,centerSize.width, centerSize.height));
            page_panel->setPosition(page_panel->getContentSize().width/2, page_panel->getContentSize().height/2);
            page_panel->setOpacity(0);
            page_panel->setName("pagePanel");
            CC_SAFE_RETAIN(page_panel);
            this->pagePanels.push_back(page_panel);
        }
        
        // パネル生成
        Sprite* panel = Sprite::create();
        Size list_size {this->pagePanels[page]->getContentSize()};
        list_size.height -= upDownMargin;
        panel->setTextureRect(Rect(0, 0, list_size.width / maxSize.x, list_size.height / maxSize.y));
        //panel->setColor(Color3B::BLACK);
        panel->setOpacity(0);
        
        Size panel_size {panel->getContentSize()};
        panel->setPosition(((i - (int)(page * maxSize.x * maxSize.y))%(int)maxSize.x) * (list_size.width / maxSize.x) + panel_size.width/2, list_size.height - ((floor((i - page * maxSize.x * maxSize.y)/(int)maxSize.x) + 1)  *  (panel_size.height)) + panel_size.height/2 + upDownMargin/2);
        
        // ページ登録
        this->pagePanels[page]->addChild(panel);
        
        // アイテム
        this->items.push_back(itr);
        Label* item = Label::createWithTTF(CsvDataManager::getInstance()->getItemData()->getItemName(itr), "fonts/cinecaption2.28.ttf", 22);
        item->setPosition(panel_size.width/2 , panel_size.height/2);
        item->setColor(Color3B::WHITE);
        item->setTag(i);
        // 不透明度を半分にしておく
        item->setCascadeOpacityEnabled(true);
        item->setOpacity(100);
        panel->addChild(item);
        
        // メニューオブジェクト登録
        this->menuObjects.push_back(item);
        
        // インクリメント
        i++;
    }
    // デフォルトセレクト
    this->onPageChanged(0);
    this->onIndexChanged(0, false);
    
    return true;
}

// アイテム説明部分生成
void ItemMenuLayer::changeItemDiscription(const int idx)
{
    // 親のスプライトを取得
    Node* bottom = this->getChildByName("bottom");
    // 子のスプライトがすでに存在すれば消してから生成
    string labelName = "discriptionLabel";
    if (bottom->getChildByName(labelName)){
        bottom->removeChildByName(labelName);
    }
    string str = LastSupper::StringUtils::strReplace("\\n", "\n", CsvDataManager::getInstance()->getItemData()->getItemDiscription(this->items[idx]));
    Label* discription = Label::createWithTTF(str, "fonts/cinecaption2.28.ttf", 24);
    //discription->setPosition(bottom->getContentSize().width / 2, leftBottom->getContentSize().height / 2);
    int margin = 15;
    discription->setPosition(discription->getContentSize().width / 2 + margin, bottom->getContentSize().height - discription->getContentSize().height / 2 - margin);
    discription->setColor(Color3B::WHITE);
    discription->setName(labelName);
    bottom->addChild(discription);
}

// 選択肢の生成
void ItemMenuLayer::createMiniSelector()
{
    // アイテムメニューのキーボードを無効化
    this->listenerKeyboard->setEnabled(false);
    // メニューラベル
    vector<string> labels;
    if (!PlayerDataManager::getInstance()->getLocalData()->isEquipedItem(this->selected_item))
    {
        labels = {"右手に装備", "左手に装備", "キャンセル"};
        this->isEquip = true;
    }
    else
    {
        labels = {"装備を外す", "キャンセル"};
        this->isEquip = false;
    }
    Point index = Point(1,labels.size()); // 要素数
    SpriteUtils::Square position = SpriteUtils::Square(74,2.5,98,22.5); // 位置
    MiniSelector::Selector selector = MiniSelector::Selector(index, position, labels);
    MiniSelector* mini = {MiniSelector::create(selector)};
    this->addChild(mini);
    mini->show();
    mini->onMiniSelectorCanceled = CC_CALLBACK_0(ItemMenuLayer::onMiniSelectorCanceled, this);
    mini->onMiniIndexSelected = CC_CALLBACK_1(ItemMenuLayer::onMiniIndexSelected, this);
    this->miniSelector = mini;
}

// ミニ選択肢が洗濯された時
void ItemMenuLayer::onMiniIndexSelected(const int idx)
{
    // 完了しましたがでている時
    if (this->miniSelector->confirm_flag)
    {
        this->onMiniSelectorCanceled();
        this->onItemMenuCanceled();
        return;
    }
    
    // キャンセルが洗濯された時
    int cancel_id = this->isEquip ? 2 : 1;
    if (idx == cancel_id)
    {
        this->onMiniSelectorCanceled();
        return;
    }
    
    // 装備処理
    if (this->isEquip)
    {
        SoundManager::getInstance()->playSE("equip.mp3");
        Direction dir = idx == 0 ? Direction::RIGHT : Direction::LEFT;
        PlayerDataManager::getInstance()->getLocalData()->setItemEquipment(dir, this->selected_item);
    }
    else
    {
        // 装備をはずす
        SoundManager::getInstance()->playSE("equip.mp3");
        Direction dir;
        if (this->selected_item == PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(Direction::RIGHT))
        {
            dir = Direction::RIGHT;
        }
        if (this->selected_item == PlayerDataManager::getInstance()->getLocalData()->getItemEquipment(Direction::LEFT))
        {
            dir = Direction::LEFT;
        }
        PlayerDataManager::getInstance()->getLocalData()->setItemEquipment(dir, 0);
        
    }
    
    // 完了メッセージ表示帯
    string equipMsg = this->isEquip ? "装備しました" : "外しました";
    NotificationBand* notification = NotificationBand::create(CsvDataManager::getInstance()->getItemData()->getItemName(this->selected_item) + "を\n" + equipMsg);
    notification->setBandColor(Color3B(64,0,0));
    this->addChild(notification);
    notification->show();
    this->notification = notification;
    this->miniSelector->confirm_flag = true;
}

// ミニセレクターを抜け出した時
void ItemMenuLayer::onMiniSelectorCanceled()
{
    SoundManager::getInstance()->playSE("back.mp3");
    this->runAction(Sequence::createWithTwoActions(
        CallFunc::create([this](){this->miniSelector->hide();}),
        CallFunc::create([this](){this->listenerKeyboard->setEnabled(true);})
    ));
    if (this->miniSelector->confirm_flag)
    {
        this->onItemMenuCanceled();
        return;
    }
}

// 表示
void ItemMenuLayer::show()
{
    this->setScale(0);
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 1.f)));
    this->listenerKeyboard->setEnabled(true);
    this->setVisible(true);
}

// 非表示
void ItemMenuLayer::hide()
{
    this->listenerKeyboard->setEnabled(false);
    if(this->notification) this->notification->hide();
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 0)));
    //this->setVisible(false);
}

// 戻る
void ItemMenuLayer::onMenuKeyPressed()
{
    FUNCLOG
    if(this->onItemMenuCanceled)
    {
        CC_SAFE_RELEASE(this->getChildByName("itemList"));
        int size = this->pagePanels.size();
        for (int i = 0; i < size; i++)
        {
            CC_SAFE_RELEASE(this->pagePanels[i]);
        }
        this->onItemMenuCanceled();
    }
}

// 決定キーを押した時
void ItemMenuLayer::onEnterKeyPressed(int idx)
{
    if(this->items[idx] < 0)
    {
        // アイテムを持っていない時
        SoundManager::getInstance()->playSE("failure.mp3");
    }
    else
    {
        // アイテムを持っている時、ミニセレクター生成
        this->selected_item = this->items[idx];
        this->createMiniSelector();
    }
}

// 選択対象が変わった時
void ItemMenuLayer::onIndexChanged(int newIdx, bool sound)
{
    // カーソル音
    if (sound)
    {
        SoundManager::getInstance()->playSE("cursorMove.mp3");
    }
    
    // カーソル処理
    for(Node* obj : this->menuObjects)
    {
        if(obj->getTag() == newIdx)
        {
            obj->runAction(FadeTo::create(0.1f, 255));
            obj->runAction(ScaleTo::create(0.2f, 1.1f));
        }
        else
        {
            obj->runAction(FadeTo::create(0.1f, 120));
            obj->runAction(ScaleTo::create(0.2f, 1.0f));
        }
    }
    // アイテム詳細を更新
    this->changeItemDiscription(newIdx);
}

// ページが変わった時
void ItemMenuLayer::onPageChanged(const int page)
{
    FUNCLOG
    //　ページ処理
    Node * itemList = this->getChildByName("itemList");
    itemList->removeChildByName("pagePanel");
    itemList->addChild(this->pagePanels[page]);
    int page_size = this->getPageSize();
    Size list_size = itemList->getContentSize();
    
    // ページカウンター
    if (itemList->getChildByName("counter"))
    {
        itemList->removeChildByName("counter");
    }
    if (page_size > 1)
    {
        Label* counter = Label::createWithTTF(to_string(page+1) + "/" + to_string(page_size), "fonts/cinecaption2.28.ttf", 20);
        counter->setPosition(counter->getContentSize().width/2 + 5 , counter->getContentSize().height/2 + 5);
        counter->setColor(Color3B::WHITE);
        counter->setName("counter");
        itemList->addChild(counter);
    }
    
    // 下への矢印
    if (itemList->getChildByName("downPager"))
    {
        itemList->removeChildByName("downPager");
    }
    if (page + 1 != page_size)
    {
        Label* downPager = Label::createWithTTF("▼", "fonts/cinecaption2.28.ttf", 16);
        downPager->setPosition(list_size.width/2 , downPager->getContentSize().height / 2 + 5);
        downPager->setColor(Color3B::WHITE);
        downPager->setName("downPager");
        itemList->addChild(downPager);
        
        // アクション生成
        this->moveUpDown(downPager);
    }
    
    // 上への矢印
    if (itemList->getChildByName("upPager"))
    {
        itemList->removeChildByName("upPager");
    }
    if (page != 0)
    {
        Label* upPager = Label::createWithTTF("▲", "fonts/cinecaption2.28.ttf", 16);
        upPager->setPosition(list_size.width/2 , list_size.height - upPager->getContentSize().height / 2 - 10);
        upPager->setColor(Color3B::WHITE);
        upPager->setName("upPager");
        itemList->addChild(upPager);
        
        // アクション生成
        this->moveUpDown(upPager);
    }
}

// 上下に反復移動
void ItemMenuLayer::moveUpDown(cocos2d::Node* target)
{
    // 移動設定
    float time = 0.3f;
    float distance = 5.f;
    
    // 初期位置の取得
    Vec2 position = target->getPosition();
    
    // 反復移動位置を設定
    ActionInterval* upDown = Sequence::createWithTwoActions(
        TargetedAction::create(target, MoveTo::create(time, Vec2(position.x, position.y + distance))),
        TargetedAction::create(target, MoveTo::create(time, position))
    );
    
    // 反復移動を登録
    this->runAction(RepeatForever::create(upDown));
}
