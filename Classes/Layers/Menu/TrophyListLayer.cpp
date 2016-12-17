//
//  TrophyListLayer.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/02.
//
//

#include "Layers/Menu/TrophyListLayer.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"
#include "Managers/PlayerDataManager.h"
#include "Models/PlayerData/GlobalPlayerData.h"
#include "Utils/SpriteUtils.h"
#include "UI/SlideNode.h"

// 初期化
bool TrophyListLayer::init()
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
    Sprite* leftTop {Sprite::createWithSpriteFrameName("menu_title_panel.png")};
    leftTop->setPosition(leftTop->getContentSize().width/2, WINDOW_HEIGHT - leftTop->getContentSize().height/2);
    this->addChild(leftTop);
    
    Label* title = Label::createWithTTF("トロフィ", "fonts/cinecaption2.28.ttf", 48);
    title->setPosition(leftTop->getContentSize().width / 2, leftTop->getContentSize().height / 2);
    title->setColor(Color3B::WHITE);
    leftTop->addChild(title);
    
    // トロフィー詳細欄
    Sprite* bottom {Sprite::createWithSpriteFrameName("item_detail.png")};
    bottom->setPosition(bottom->getContentSize().width/2, bottom->getContentSize().height/2);
    bottom->setName("bottom");
    this->addChild(bottom);
    
    // 最高記録
    Sprite* rightTop {Sprite::createWithSpriteFrameName("item_equipment.png")};
    rightTop->setPosition(rightTop->getContentSize().width/2 + parcent.width * 30, rightTop->getContentSize().height/2 + parcent.height * 80);
    this->addChild(rightTop);
    
    Label* rightup_title = Label::createWithTTF("最高クリア記録", "fonts/cinecaption2.28.ttf", 26);
    rightup_title->setPosition(rightup_title->getContentSize().width/2 + 30, rightTop->getContentSize().height - rightup_title->getContentSize().height/2 - 15);
    rightup_title->setColor(Color3B::WHITE);
    rightTop->addChild(rightup_title);
    
    // 記録の取得
    PlayerDataManager* pdm = PlayerDataManager::getInstance();
    string fast_time = LastSupper::StringUtils::getTimeForDisplay(pdm->getGlobalData()->getBestClearTimeSecound());
    string min_save = to_string(pdm->getGlobalData()->getBestSaveCount()) + "回";
    Label* equipment = Label::createWithTTF("最速クリア時間 : " + fast_time + "\n最小セーブ回数 : " + min_save, "fonts/cinecaption2.28.ttf", 26);
    equipment->setPosition(equipment->getContentSize().width/2 + 20, equipment->getContentSize().height/2 + 20);
    equipment->setColor(Color3B::WHITE);
    rightTop->addChild(equipment);
    
    // トロフィーリスト
    square = SpriteUtils::Square(0,25,100,80);
    margin = SpriteUtils::Margin(1.0);
    Sprite* center = SpriteUtils::getSquareSprite(square, margin);
    center->setColor(Color3B::BLACK);
    //Sprite* center {Sprite::createWithSpriteFrameName("item_selector.png")};
    //center->setPosition(center->getContentSize().width/2, center->getContentSize().height/2 + parcent.height * 25);
    center->setName("itemList");
    this->addChild(center);
    
    // メニュー設定
    vector<int> trophies = CsvDataManager::getInstance()->getTrophyData()->getIdAll();
    Point size {Point(2,5)};
    
    if (!MenuLayer::init(size, trophies.size(), center)) return false;
    
    int i = 0;
    int page = 0;
    int upDownMargin = 40;
    Size centerSize {center->getContentSize()};
    
    for(int trophy_id : trophies)
    {
        // ページパネル生成
        page = floor(i / (size.x * size.y));
        
        // トロフィーパネル生成
        Sprite* panel = Sprite::create();
        Size list_size {this->pagePanels[page]->getContentSize()};
        list_size.height -= upDownMargin;
        panel->setTextureRect(Rect(0, 0, list_size.width / size.x, list_size.height / size.y));
        panel->setOpacity(0);
        
        Size panel_size {panel->getContentSize()};
        panel->setPosition(((i - (int)(page * size.x * size.y))%(int)size.x) * (list_size.width / size.x) + panel_size.width/2, list_size.height - ((floor((i - page * size.x * size.y)/(int)size.x) + 1)  *  (panel_size.height)) + panel_size.height/2 + upDownMargin/2);
        
        // ページに登録
        this->pagePanels[page]->addChild(panel);
        
        // トロフィー画像
        Sprite* trophy_img {Sprite::createWithSpriteFrameName("trophy_gold.png")};
        trophy_img->setPosition(trophy_img->getContentSize().width, trophy_img->getContentSize().height/2);
        trophy_img->setScale(0.8);
        
        // トロフィー名
        this->trophies.push_back(trophy_id);
        string trophy_name = "? ? ? ? ?";
        if (PlayerDataManager::getInstance()->getGlobalData()->hasTrophy(trophy_id))
        {
            trophy_name = CsvDataManager::getInstance()->getTrophyData()->getName(trophy_id);
            
            // トロフィー画像
            panel->addChild(trophy_img);
        }
        Label* trophy = Label::createWithTTF(trophy_name, "fonts/cinecaption2.28.ttf", 22);
        trophy->setPosition(trophy->getContentSize().width/2 + trophy_img->getContentSize().width * 2, panel_size.height/2);
        trophy->setColor(Color3B::WHITE);
        trophy->setTag(i);
        // 不透明度を半分にしておく
        trophy->setCascadeOpacityEnabled(true);
        trophy->setOpacity(100);
        panel->addChild(trophy);
        
        // メニューオブジェクト登録
        this->menuObjects.push_back(trophy);
        
        // インクリメント
        i++;
    }
    // デフォルトセレクト
    this->onPageChanged(0);
    this->onIndexChanged(0, false);
    
    return true;
}

// トロフィー説明部分生成
void TrophyListLayer::changeTrophyDiscription(const int idx)
{
    // 親のスプライトを取得
    Node* bottom = this->getChildByName("bottom");
    // 子のスプライトがすでに存在すれば消してから生成
    string labelName = "discriptionLabel";
    if (bottom->getChildByName(labelName)){
        bottom->removeChildByName(labelName);
    }
    
    // パネル作成
    Node* label_panel {Node::create()};
    label_panel->setName(labelName);
    bottom->addChild(label_panel);
    
    // 条件
    int margin = 15;
    string str = LastSupper::StringUtils::strReplace("\\n", "\n", CsvDataManager::getInstance()->getTrophyData()->getCondition(this->trophies[idx]));
    Label* condition = Label::createWithTTF(str, "fonts/cinecaption2.28.ttf", 28);
    condition->setPosition(bottom->getContentSize().width / 2, bottom->getContentSize().height - condition->getContentSize().height / 2 - margin);
    condition->setColor(Color3B::WHITE);
    label_panel->addChild(condition);
    
    // コメント
    string comment_str = PlayerDataManager::getInstance()->getGlobalData()->hasTrophy(idx + 1) ? "\n  「" + LastSupper::StringUtils::strReplace("\\n", "\n", CsvDataManager::getInstance()->getTrophyData()->getComment(this->trophies[idx])) + "」" : "";
    Label* comment = Label::createWithTTF(comment_str, "fonts/cinecaption2.28.ttf", 28);
    comment->setPosition(bottom->getContentSize().width / 2, bottom->getContentSize().height - condition->getContentSize().height - comment->getContentSize().height / 2 - margin / 2);
    comment->setColor(Color3B::WHITE);
    label_panel->addChild(comment);
}

// 表示
void TrophyListLayer::show()
{
    this->setScale(0);
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 1.f)));
    this->listenerKeyboard->setEnabled(true);
    this->setVisible(true);
}

// 非表示
void TrophyListLayer::hide()
{
    this->listenerKeyboard->setEnabled(false);
    this->runAction(EaseCubicActionOut::create(ScaleTo::create(0.3f, 0)));
    //this->setVisible(false);
}

// 戻る
void TrophyListLayer::onMenuKeyPressed()
{
    FUNCLOG
    if(this->onTrophyListCanceled)
    {
        this->onTrophyListCanceled();
    }
}

// スペースキーを押した時
void TrophyListLayer::onEnterKeyPressed(int idx)
{
    int trophyId = idx + 1;

    // トロフィー所持していない場合
    if (!PlayerDataManager::getInstance()->getGlobalData()->hasTrophy(trophyId)) {
        SoundManager::getInstance()->playSE(Resource::SE::FAILURE);
        return;
    }
    
    // データ取得
    TrophyData* trophyData { CsvDataManager::getInstance()->getTrophyData() };
    string imgName = trophyData->getImagePath(trophyId);
    string voiceFile = trophyData->getVoicePath(trophyId);
    
    // ボイスがない場合
    if (voiceFile == "") {
        SoundManager::getInstance()->playSE(Resource::SE::BACK);
        return;
    }
    
    // 画像がある場合
    if (SpriteFrameCache::getInstance()->getSpriteFrameByName(imgName)) {
        // 背景準備
        SpriteUtils::Square square = SpriteUtils::Square(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
        SpriteUtils::Margin margin = SpriteUtils::Margin(0.0);
        Sprite* background { SpriteUtils::getSquareSprite(square,margin) };
        background->setColor(Color3B::BLACK);
        background->setOpacity(172);
        
        // 画像とスライドノードの準備
        Sprite* img { Sprite::createWithSpriteFrameName(imgName) };
        Point outPosition { Point(WINDOW_WIDTH + img->getContentSize().width/2, 0) };
        Point inPosition { Point(WINDOW_WIDTH - img->getContentSize().width/2, 0) };
        SlideNode* slideBase { SlideNode::create(inPosition, outPosition) };
        
        // Spriteをセット
        slideBase->addChild(img);
        background->addChild(slideBase);
        this->addChild(background);
        
        // アニメーション
        this->listenerKeyboard->setEnabled(false);
        slideBase->slideIn();
        SoundManager::getInstance()->playVoice(voiceFile, 1.0, [this,slideBase,background](int voiceId, const string& fileName) {
            slideBase->slideOut([this,background](SlideNode* slideBase) {
                slideBase->removeAllChildren();
                slideBase->removeFromParent();
                background->removeFromParent();
                this->listenerKeyboard->setEnabled(true);
            });
        });
        
        return;
    }
    
    // サウンドだけ出す
    SoundManager::getInstance()->playVoice(voiceFile);
}

// 選択対象が変わった時
void TrophyListLayer::onIndexChanged(int newIdx, bool sound)
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
    this->changeTrophyDiscription(newIdx);
}

