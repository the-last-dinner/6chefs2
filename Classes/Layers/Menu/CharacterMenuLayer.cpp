//
//  CharacterMenuLayer.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/09.
//
//

#include "Layers/Menu/CharacterMenuLayer.h"
#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "UI/SlideNode.h"

CharacterMenuLayer::CharacterMenuLayer(){FUNCLOG}

CharacterMenuLayer::~CharacterMenuLayer(){FUNCLOG}

// 初期化
bool CharacterMenuLayer::init()
{
    
    if (!MenuLayer::init(1,12)) return false;
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
    
    Label* title = Label::createWithTTF("キャラ", "fonts/cinecaption2.28.ttf", 48);
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
    
    Size list_size {right->getContentSize()};
    float space = 2;
    Point maxSize {Point(1,12)};
    this->characters = CsvDataManager::getInstance()->getDisplayCharacters();
    int chara_count = this->characters.size();
    for (int i=0;i<chara_count; i++){
        // パネル生成
        Sprite* panel = Sprite::create();
        panel->setTextureRect(Rect(0, 0, list_size.width / maxSize.x, list_size.height / maxSize.y - space));
        panel->setOpacity(0);
        Size panel_size {panel->getContentSize()};
        panel->setPosition((i%(int)maxSize.x) * (list_size.width / maxSize.x) + panel_size.width/2, list_size.height - ((floor(i/(int)maxSize.x) + 1)  *  (panel_size.height)) + panel_size.height/2 - space * 5);
        right->addChild(panel);
        
        // キャラクター名
        string chara_name {""};
        if (PlayerDataManager::getInstance()->getLocalData()->getCharacterProfileLevel(this->characters[i]) < 0)
        {
            chara_name = "? ? ? ? ?";
        }
        else
        {
            chara_name = CsvDataManager::getInstance()->getCharaNameWithRuby(this->characters[i]);
        }
        
        // ラベル生成
        Label* chara = Label::createWithTTF(chara_name, "fonts/cinecaption2.28.ttf", 24);
        chara->setPosition(panel_size.width/2 , panel_size.height / 2);
        chara->setColor(Color3B::WHITE);
        chara->setTag(i);
        // 不透明度を抑えるしておく
        chara->setCascadeOpacityEnabled(true);
        chara->setOpacity(120);
        panel->addChild(chara);
        
        // メニューオブジェクトに登録
        this->menuObjects.push_back(chara);
    }
    
    // デフォルトセット
    onIndexChanged(0, false);
    
    return true;
}

// カーソル移動
void CharacterMenuLayer::onIndexChanged(int newIdx, bool sound)
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

void CharacterMenuLayer::changeCharaImage(const int idx)
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
    
    if (PlayerDataManager::getInstance()->getLocalData()->getCharacterProfileLevel(this->characters[idx]) < 0)
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
        string fileName = CsvDataManager::getInstance()->getCharaFileName(this->characters[idx]) + "_s_0.png";
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
        int level = PlayerDataManager::getInstance()->getLocalData()->getFriendship(this->characters[idx]);
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
void CharacterMenuLayer::onMenuKeyPressed()
{
    // キャラ説明が出ている場合は消すだけ
    if (this->isDiscription)
    {
        this->onSpacePressed(0);
        return;
    }
    
    // キャラクターメニューを削除
    if (this->onCharacterMenuCanceled)
    {
        this->onCharacterMenuCanceled();
    }
}

// スペースキー
void CharacterMenuLayer::onSpacePressed(int idx)
{
    // 見れないレベルのキャラの時
    if (PlayerDataManager::getInstance()->getLocalData()->getCharacterProfileLevel(this->characters[idx]) < 0)
    {
        SoundManager::getInstance()->playSE("failure.mp3");
        return;
    }
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
        Size panel_size = Size(back->getContentSize().width, back->getContentSize().height/3);
        int canCheckLevel = PlayerDataManager::getInstance()->getLocalData()->getCharacterProfileLevel(this->characters[idx]);
        for(int i=0;i<3; i++)
        {
            string profile {""};
            // 見れるレベルのプロフィールかチェック
            if (canCheckLevel >= i)
            {
                profile = LastSupper::StringUtils::strReplace("\\n", "\n", CsvDataManager::getInstance()->getCharaDiscription(this->characters[idx], i));
            }
            else
            {
                profile = "? ? ? ? ?";
            }
            Label* label = Label::createWithTTF(profile, "fonts/cinecaption2.28.ttf", 24);
            label->setColor(Color3B::WHITE);
            label->setPosition(canCheckLevel >= i ? label->getContentSize().width/2 + 20 : panel_size.width/2, (2-i) * panel_size.height + panel_size.height/2);
            discriptions.push_back(label);
            back->addChild(label);
        }
        this->isDiscription = true;
        this->setCursorEnable(false);
    }
}


// 表示
void CharacterMenuLayer::show()
{
    this->listenerKeyboard->setEnabled(true);
    this->setVisible(true);
    this->setScale(0);
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 1.f)));
}

// 非表示
void CharacterMenuLayer::hide()
{
    this->listenerKeyboard->setEnabled(false);
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 0)));
    //this->setVisible(false);
}