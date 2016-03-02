//
//  EndingScene.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/03.
//
//

#include "Scenes/EndingScene.h"

#include "Datas/Scene/EndingSceneData.h"
#include "Datas/Scene/DungeonSceneData.h"
#include "Datas/Scene/TitleSceneData.h"
#include "Scenes/DungeonScene.h"
#include "Scenes/TitleScene.h"
#include "Layers/LoadingLayer.h"

// コンストラクタ
EndingScene::EndingScene() {FUNCLOG};

// デストラクタ
EndingScene::~EndingScene() {FUNCLOG};

// 初期化
bool EndingScene::init(const int endingId)
{
    this->end_id = endingId;
    return BaseScene::init(EndingSceneData::create());
}

// シーン切り替え終了時
void EndingScene::onEnter()
{
    BaseScene::onEnter();
}

// リソースのプリロード完了時
void EndingScene::onPreloadFinished(LoadingLayer* loadingLayer)
{
    // プリロード終了
    loadingLayer->onLoadFinished();
    
    // エンディング振り分け
    switch (this->end_id) {
        case etoi(END_ID::NORMAL_END):
            createNormalEnding();
            break;
        case etoi(END_ID::TRUE_END):
            createTrueEnding();
            break;
        default:
            createBadEnding();
            break;
    }
}

// トゥルーエンディング生成
void EndingScene::createTrueEnding()
{
    vector<pair<string,float>> credits_name = {
        // ------------------
        {"--- BGM素材 ---",15.0},
        //
        {"魔王魂", 17.0},
        //
        {"クラシック名曲", 18.5},
        {"サウンドライブラリー", 19.0},
        // ------------------
        {"--- 効果音素材 ---", 23.0},
        //
        {"効果音ラボ", 24.5},
        //
        {"ポケットサウンド", 26.0},
        //
        {"フリー音楽素材", 27.5},
        {"Senses Circuit", 28.0},
        // ------------------
        {"--- 画像素材 ---", 32.0},
        //
        {"ドット絵世界", 33.5},
        //
        {"すきまの素材", 35.0},
        //
        {"臼井の会", 36.5},
        //
        {"ぴぽや",38.0},
        //
        {"-RyU-", 39.5},
        //
        {"sorairobaibai", 41.0},
        //
        {"とまりぎ", 42.5},
        //
        {"水晶と月の家", 44.0},
        //
        {"First Seed Material", 45.5},
        //
        {"なにかしらツクール", 47.0},
        //
        {"尾羽の小屋", 48.5},
        //
        {"ぼう然の流もの喫茶", 50.0},
        //
        {"白螺子屋", 51.5},
        // ------------------
        {"--- 制作メンバー ---", 55.0},
        //
        {"ジェダイマスター", 57.0},
        {"エンジニア", 57.5},
        {"ナニヲ", 58.5},
        //
        {"真夏大好き", 63.5},
        {"エンジニア",64.0},
        {"いのす", 65.0},
        //
        {"孫一ペロペロ", 70.0},
        {"キャラクターデザイナー", 70.5},
        {"早乙女", 71.5},
        //
        {"ピクセル", 76.5},
        {"アート", 77.0},
        {"おぐぐ", 78.0},
        //
        {"マエストロ", 83.0},
        {"プランナー", 83.5},
        {"スズラン", 84.5},
    };
    vector<string> pictures_name = {
        "epilogue.png",
        "chapter1.png",
        "daigoro_usually.png",
        "dandan_usually.png",
        "chapter2.png",
        "nadeshiko_usually.png",
        "ranmaru_usually.png",
        "chapter3.png",
        "yuki_stan.png",
        "manaka_usually.png",
        "chapter4.png",
        "taihou_usually.png",
        "chapter5.png",
        "magoichi_usually.png",
    };
    
    // 背景
    Sprite* background {Sprite::createWithSpriteFrameName("background.png")};
    background->setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    this->addChild(background);
    
    // 文字
    Vector<FiniteTimeAction*> label_acts {};
    label_acts.pushBack(DelayTime::create(15.f));
    int credits_len = credits_name.size();
    
    float x = WINDOW_WIDTH * 4 / 5 - 10;
    float font_size = 28;
    float moving_time = 9.f;
    for (int i = 0; i < credits_len; i++)
    {
        Label* label {Label::createWithTTF(credits_name[i].first, Resource::Font::MESSAGE, font_size)};
        label->setPosition(x, -1 * label->getContentSize().height / 2);
        label->setColor(Color3B::WHITE);
        this->addChild(label);
        label_acts.pushBack(Sequence::createWithTwoActions(TargetedAction::create(label, DelayTime::create(credits_name[i].second)), TargetedAction::create(label, MoveTo::create(credits_name[i].second < 55 ? moving_time : (moving_time + 1.0), Vec2(label->getPosition().x, label->getContentSize().height / 2 + WINDOW_HEIGHT)))));
    }
    
    // ロゴの位置調整
    float pos = 22.5;
    
    // 制作
    Label* last_label {Label::createWithTTF("制作", Resource::Font::MESSAGE, font_size)};
    last_label->setPosition(x, -1 * last_label->getContentSize().height / 2);
    last_label->setColor(Color3B::WHITE);
    this->addChild(last_label);
    last_label->runAction(Sequence::createWithTwoActions(DelayTime::create(91.0), MoveTo::create(4.9, Vec2(last_label->getPosition().x, last_label->getContentSize().height * 2.5 + WINDOW_HEIGHT / 2 - pos))));
    
    // ロゴ
    Sprite* logo {Sprite::createWithSpriteFrameName("the_last_dinner_log.png")};
    logo->setPosition(x, -1 * logo->getContentSize().height / 2);
    float scale_logo = 0.5;
    logo->setScale(scale_logo);
    this->addChild(logo);
    logo->runAction(Sequence::createWithTwoActions(DelayTime::create(91.5), MoveTo::create(4.4, Vec2(logo->getPosition().x, WINDOW_HEIGHT/2 - pos))));
    
    // 画像
    vector<Sprite*> pictures {};
    int pic_len = pictures_name.size();
    
    Sprite* sprite {Sprite::create(Resource::SpriteFrame::BASE_PATH + "disp/" + pictures_name[0])};
    sprite->setOpacity(0);
    sprite->setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    this->addChild(sprite);
    pictures.push_back(sprite);
    
    x = WINDOW_WIDTH / 3 - 20;
    float y = WINDOW_HEIGHT / 2;
    float scale = 0.5;
    for (int i = 1; i < pic_len; i++)
    {
        Sprite* sprite {Sprite::create(Resource::SpriteFrame::BASE_PATH + "disp/" + pictures_name[i])};
        sprite->setOpacity(0);
        sprite->setScale(scale);
        sprite->setPosition(x, y);
        this->addChild(sprite);
        pictures.push_back(sprite);
    }
    
    // 画像の動き
    Vector<FiniteTimeAction*> picture_acts {};
    picture_acts.pushBack(TargetedAction::create(pictures[0], FadeIn::create(1.f)));
    picture_acts.pushBack(TargetedAction::create(pictures[0], DelayTime::create(13.f)));
    picture_acts.pushBack(TargetedAction::create(pictures[0], FadeOut::create(1.f)));
    for (int i = 1; i < pic_len; i++)
    {
        picture_acts.pushBack(TargetedAction::create(pictures[i], FadeIn::create(1.f)));
        if (i != pic_len - 1)
        {
            picture_acts.pushBack(TargetedAction::create(pictures[i], DelayTime::create(4.7f)));
            picture_acts.pushBack(TargetedAction::create(pictures[i], FadeOut::create(1.f)));
        }
    }
    
    // エディング実行
    SoundManager::getInstance()->playBGM("ending.mp3", false, 2.0);
    this->runAction(Sequence::create(picture_acts));
    this->runAction(Spawn::create(label_acts));
    this->runAction(Sequence::create(DelayTime::create(103), CallFunc::create([this](){
        this->onEndingFinished();
    }), nullptr));
}

// ノーマルエンドを生成
void EndingScene::createNormalEnding()
{
    Label* label {Label::createWithTTF("MEDIUM END", Resource::Font::SYSTEM, 80)};
    label->setColor(Color3B::WHITE);
    label->setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    label->setOpacity(0);
    this->addChild(label);
    label->runAction(Sequence::create(FadeIn::create(2.f), TintTo::create(2.f, Color3B::RED), DelayTime::create(2.f), CallFunc::create([this](){this->onEndingFinished();}), nullptr));
}

// バッドエンドを生成
void EndingScene::createBadEnding()
{
    Label* label {Label::createWithTTF("BAD END", Resource::Font::SYSTEM, 80)};
    label->setColor(Color3B::WHITE);
    label->setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    label->setOpacity(0);
    this->addChild(label);
    label->runAction(Sequence::create(FadeIn::create(2.f), TintTo::create(2.f, Color3B::RED), DelayTime::create(2.f), CallFunc::create([this](){this->onEndingFinished();}), nullptr));
    
}

// エンディングが終了した時
void EndingScene::onEndingFinished()
{
    // FadeOut用幕
    SpriteUtils::Square square = SpriteUtils::Square(0,0,100,100);
    Sprite* black {SpriteUtils::getSquareSprite(square, 0)};
    black->setOpacity(0);
    black->setColor(Color3B::BLACK);
    this->addChild(black);
    
    // ゲームクリアをセット
    this->runAction(Sequence::create(
                                     TargetedAction::create(black, FadeIn::create(2.f)),
                                     CallFunc::create([this](){
            PlayerDataManager::getInstance()->setGameClear(this->end_id);
            this->replaceScene();
        }), nullptr));
    
}

// シーンを切り替える
void EndingScene::replaceScene()
{
    BaseScene* target {nullptr};
    switch (this->end_id) {
        case etoi(END_ID::TRUE_END):
            target = DungeonScene::create(DungeonSceneData::create(Location(3,20,39,Direction::BACK)));
            break;
        case etoi(END_ID::NORMAL_END):
            target = DungeonScene::create(DungeonSceneData::create(Location(39,16,5,Direction::FRONT)));
            break;
        default:
            target = TitleScene::create();
            break;
    }
    Director::getInstance()->replaceScene(target);
}