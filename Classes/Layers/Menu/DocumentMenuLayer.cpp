//
//  DocumentMenuLayer.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/09.
//
//

#include "Layers/Menu/DocumentMenuLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "UI/SlideNode.h"

DocumentMenuLayer::DocumentMenuLayer(){FUNCLOG}

DocumentMenuLayer::~DocumentMenuLayer(){FUNCLOG}

// 初期化
bool DocumentMenuLayer::init()
{
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
    
    Label* title = Label::createWithTTF("資料", "fonts/cinecaption2.28.ttf", 48);
    title->setPosition(leftTop->getContentSize().width / 2, leftTop->getContentSize().height / 2);
    title->setColor(Color3B::WHITE);
    leftTop->addChild(title);
    
    // キャラ紹介
    //square = SpriteUtils::Square(0,0,30,80);
    //margin = SpriteUtils::Margin(1.5,1.5,3.0,3.0);
    //Sprite* leftBottom = SpriteUtils::getSquareSprite(square, margin);
    //leftBottom->setColor(Color3B::BLACK);
    Sprite* leftBottom {Sprite::createWithSpriteFrameName("character_discription.png")};
    leftBottom->setPosition(leftBottom->getContentSize().width/2, leftBottom->getContentSize().height/2);
    leftBottom->setName("charaImage");
    this->addChild(leftBottom);
    
    // キャラ選択部分
    //square = SpriteUtils::Square(30,0,100,100);
    //margin = SpriteUtils::Margin(3.0,3.0,3.0,1.5);
    //Sprite* right = SpriteUtils::getSquareSprite(square, margin);
    //right->setColor(Color3B::BLACK);
    Sprite* right {Sprite::createWithSpriteFrameName("character_selector.png")};
    right->setPosition(right->getContentSize().width/2 + parcent.width * 30, right->getContentSize().height/2);
    right->setName("charaRight");
    this->addChild(right);
    
    // メニュー設定
    DocumentData* docData { CsvDataManager::getInstance()->getDocumentData() };
    int docCount = docData->getDocumentCount();
    this->documentIds = docData->getDocumentDataIds();
    Point maxSize {Point(1,12)};
    if (!MenuLayer::init(maxSize, docCount, right)) return false;
    
    int page = 0;
    int upDownMargin = 40;
    Size listSize {right->getContentSize()};
    listSize.height -= upDownMargin;
    for (int i = 0; i < docCount; i++)
    {
        // ページパネル生成
        page = floor(i / (maxSize.x * maxSize.y));

        // 資料パネル生成
        Sprite* panel = Sprite::create();
        
        panel->setTextureRect(Rect(0, 0, listSize.width / maxSize.x, listSize.height / maxSize.y));
        panel->setOpacity(0);
        Size panelSize {panel->getContentSize()};
        // panel->setPosition((i%(int)maxSize.x) * (listSize.width / maxSize.x) + panelSize.width/2, listSize.height - ((floor(i/(int)maxSize.x) + 1)  *  (panelSize.height)) + panelSize.height/2 - space * 5);
        panel->setPosition(((i - (int)(page * maxSize.x * maxSize.y))%(int)maxSize.x) * (listSize.width / maxSize.x) + panelSize.width/2, listSize.height - ((floor((i - page * maxSize.x * maxSize.y)/(int)maxSize.x) + 1)  *  (panelSize.height)) + panelSize.height/2 + upDownMargin/2);
        
        // ページに登録
        this->pagePanels[page]->addChild(panel);
        
        // キャラクター名
        string docName {""};
        if (PlayerDataManager::getInstance()->getLocalData()->getCharacterProfileLevel(this->documentIds[i]) < 0)
        {
            docName = "? ? ? ? ?";
        }
        else
        {
            docName = docData->getName(this->documentIds[i]);
        }
        
        // ラベル生成
        Label* doc = Label::createWithTTF(docName, "fonts/cinecaption2.28.ttf", 24);
        doc->setPosition(panelSize.width/2 , panelSize.height / 2);
        doc->setColor(Color3B::WHITE);
        doc->setTag(i);
        // 不透明度を抑えるしておく
        doc->setCascadeOpacityEnabled(true);
        doc->setOpacity(120);
        panel->addChild(doc);
        
        // メニューオブジェクトに登録
        this->menuObjects.push_back(doc);
    }
    
    // デフォルトセット
    this->onPageChanged(0);
    this->onIndexChanged(0, false);
    
    return true;
}

// カーソル移動
void DocumentMenuLayer::onIndexChanged(int newIdx, bool sound)
{
    if (sound)
    {
        SoundManager::getInstance()->playSE("cursorMove.mp3");
    }
    for(Node* obj : this->menuObjects)
    {
        if(obj->getTag() == newIdx)
        {
            obj->runAction(FadeTo::create(0.2f, 255));
            obj->runAction(ScaleTo::create(0.2f, 1.2f));
        }
        else
        {
            obj->runAction(FadeTo::create(0.2f, 120));
            obj->runAction(ScaleTo::create(0.2f, 1.0f));
        }
    }
    // キャラクターイメージを変更
    this->changeCharaImage(newIdx);
}

void DocumentMenuLayer::changeCharaImage(const int idx)
{
    // 親のスプライトを取得
    Node* leftBottom = this->getChildByName("charaImage");
    // 子のスプライトがすでに存在すれば消してから生成
    string labelName = "imgLabel";
    if (leftBottom->getChildByName(labelName)){
        leftBottom->removeChildByName(labelName);
    }
    Size panel = leftBottom->getContentSize();
    
    // ハートパネルの削除
    string panelName = "heartPanel";
    if (leftBottom->getChildByName(panelName))
    {
        leftBottom->removeChildByName(panelName);
    }
    
    DocumentData* docData {CsvDataManager::getInstance()->getDocumentData()};
    if (PlayerDataManager::getInstance()->getLocalData()->getCharacterProfileLevel(this->documentIds[idx]) < 0)
    {
        // 見ることができないキャラクター
        Label* label = Label::createWithTTF("? ? ? ? ?", "fonts/cinecaption2.28.ttf", 24);
        label->setPosition(panel.width/2, panel.height/2);
        label->setName(labelName);
        leftBottom->addChild(label);
    }
    else
    {
        // 見ることができるキャラクター
        string fileName = docData->getImageFileName(this->documentIds[idx]);
        if(SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName))
        {
            Sprite* img { Sprite::createWithSpriteFrameName(fileName)};
            img->setScale(0.30);
            img->setPosition(panel.width/2,panel.height/2 + 20);
            img->setName(labelName);
            //img->setLocalZOrder(-1);
            leftBottom->addChild(img);
        }
        else
        {
            Label* label = Label::createWithTTF("NO IMAGE", "fonts/cinecaption2.28.ttf", 24);
            label->setPosition(panel.width/2, panel.height/2);
            label->setName(labelName);
            leftBottom->addChild(label);
        }
        // 友好度をセット
        int level = PlayerDataManager::getInstance()->getLocalData()->getFriendship(this->documentIds[idx]);
        if (level < 0) return; // 友好度が存在しない時はリターン
        // ハートを置くパネルの生成
        Sprite* heart_panel = Sprite::create();
        heart_panel->setPosition(heart_panel->getContentSize().width/2, heart_panel->getContentSize().height/2);
        heart_panel->setName(panelName);
        leftBottom->addChild(heart_panel);
        // ハート生成
        for (int i=0; i<3; i++)
        {
            string file_name = level>=i ? "heart_pink.png" : "heart_black.png";
            Sprite* heart {Sprite::createWithSpriteFrameName(file_name)};
            heart->setScale(0.50);
            heart->setOpacity(180);
            heart->setPosition(i * panel.width / 3 + panel.width / 6, heart->getContentSize().height / 4 + 15);
            heart_panel->addChild(heart);
        }
        
    }
}

// メニューキー
void DocumentMenuLayer::onMenuKeyPressed()
{
    // キャラ説明が出ている場合は消すだけ
    if (this->isDiscription)
    {
        this->onEnterKeyPressed(-1);
        return;
    }
    
    // キャラクターメニューを削除
    if (this->onCharacterMenuCanceled)
    {
        this->onCharacterMenuCanceled();
    }
}

// 決定キー
void DocumentMenuLayer::onEnterKeyPressed(int idx)
{
    // 状態によって場合分け
    if (this->isDiscription)
    {
        // キャラ説明を削除
        SoundManager::getInstance()->playSE(Resource::SE::BACK);
        this->slideNode->slideOut([this](SlideNode* slideNode){this->removeChildByName("charaBack");});
        this->isDiscription = false;
        this->setCursorEnable(true);
    }
    else
    {
        // 見れないレベルのキャラの時
        if (PlayerDataManager::getInstance()->getLocalData()->getCharacterProfileLevel(this->documentIds[idx]) < 0)
        {
            SoundManager::getInstance()->playSE("failure.mp3");
            return;
        }
        
        SoundManager::getInstance()->playSE(Resource::SE::TITLE_ENTER);
        // 背景の作成
        Sprite* back {Sprite::createWithSpriteFrameName("character_selector.png")};
        Point inPosition {Point(back->getContentSize().width/2 + (WINDOW_WIDTH/100) * 30, back->getContentSize().height/2)};
        Point OutPosition {Point(back->getContentSize().width * 3/2 + (WINDOW_WIDTH/100) * 30, back->getContentSize().height/2)};
        SlideNode* sNode {SlideNode::create(inPosition, OutPosition)};
        sNode->setName("charaBack");
        this->addChild(sNode);
        sNode->addChild(back);
        this->slideNode = sNode;
        this->slideNode->slideIn();
        
        // 詳細のラベルを作成
        vector<Label*> discriptions;
        Size panelSize = Size(back->getContentSize().width, back->getContentSize().height/3);
        int canCheckLevel = PlayerDataManager::getInstance()->getLocalData()->getCharacterProfileLevel(this->documentIds[idx]);
        for(int i = 0; i < 3; i++)
        {
            string profile {""};
            // 見れるレベルのプロフィールかチェック
            if (canCheckLevel >= i)
            {
                profile = LastSupper::StringUtils::strReplace("\\n", "\n", CsvDataManager::getInstance()->getDocumentData()->getDiscription(this->documentIds[idx], i));
            }
            else
            {
                profile = "? ? ? ? ?";
            }
            Label* label = Label::createWithTTF(profile, "fonts/cinecaption2.28.ttf", 24);
            label->setColor(Color3B::WHITE);
            label->setPosition(canCheckLevel >= i ? label->getContentSize().width/2 + 20 : panelSize.width/2, (2-i) * panelSize.height + panelSize.height/2);
            discriptions.push_back(label);
            back->addChild(label);
        }
        this->isDiscription = true;
        this->setCursorEnable(false);
    }
}


// 表示
void DocumentMenuLayer::show()
{
    this->listenerKeyboard->setEnabled(true);
    this->setVisible(true);
    this->setScale(0);
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 1.f)));
}

// 非表示
void DocumentMenuLayer::hide()
{
    this->listenerKeyboard->setEnabled(false);
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 0)));
    //this->setVisible(false);
}