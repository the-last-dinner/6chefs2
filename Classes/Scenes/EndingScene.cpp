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
#include "Layers/EventListener/ConfigEventListenerLayer.h"

const string EndingScene::ENDING_FILE = "config/Ending";
const char* EndingScene::JSON_MEMBER_TYPE = "type";
const char* EndingScene::JSON_MEMBER_TEXT = "text";
const char* EndingScene::JSON_MEMBER_CREDITS = "credits";
const char* EndingScene::JSON_MEMBER_TIME = "time";
const char* EndingScene::JSON_MEMBER_PICTURES = "pictures";
const char* EndingScene::JSON_MEMBER_NAME = "name";
const char* EndingScene::JSON_MEMBER_BACKGROUND = "background";
const char* EndingScene::JSON_MEMBER_LAST_TEXT = "lastText";
const char* EndingScene::JSON_MEMBER_LAST_PICTURE = "lastPicture";
const char* EndingScene::JSON_MEMBER_BGM = "bgm";
// コンストラクタ
EndingScene::EndingScene() {FUNCLOG};

// デストラクタ
EndingScene::~EndingScene() {FUNCLOG};

// 初期化
bool EndingScene::init(const int endingId)
{
    if(!BaseScene::init(EndingSceneData::create())) return false;
    
    _configListener->setKeyconfigEnabled(false);
    _end_id = endingId;
    
    return true;
}

// pushされた時
void EndingScene::onEnter()
{
    BaseScene::onEnter();
    this->onEnterPushedScene();
}

// popされた時
void EndingScene::onExit()
{
    BaseScene::onExit();
    this->onExitPushedScene();
}

// リソースのプリロード完了時
void EndingScene::onPreloadFinished(LoadingLayer* loadingLayer)
{
    // プリロード終了
    loadingLayer->onLoadFinished();
    
    // idからjsonを取得する
    rapidjson::Value& json = LastSupper::JsonUtils::readJsonCrypted(FileUtils::getInstance()->fullPathForFilename(ENDING_FILE + ES_EXTENSION)).FindMember(to_string(_end_id).c_str())->value;
    
    // エンディング振り分け
     map<string, function<void(rapidjson::Value&)>> createEndings = {
        {"normal", [this](rapidjson::Value& json){createNormalEnding(json);}},
        {"special", [this](rapidjson::Value& json){createSpecialEnding(json);}},
    };
    
    if(!json.HasMember(JSON_MEMBER_TYPE)) LastSupper::AssertUtils::fatalAssert("EndingScriptError\ntypeが存在しません");
    string typeName = json[JSON_MEMBER_TYPE].GetString();
    
    if(createEndings.count(typeName) == 0) LastSupper::AssertUtils::warningAssert("EndingScriptError\n" + typeName + "は存在しません");
    
    createEndings.at(typeName)(json);
    
}

// 特別なエンディング生成
void EndingScene::createSpecialEnding(rapidjson::Value& json)
{
    vector<pair<string,float>> credits_name = {};
    if(!json.HasMember(JSON_MEMBER_CREDITS)) LastSupper::AssertUtils::fatalAssert("EndingScriptError\ncreditsが存在しません");
    rapidjson::Value& creditsJson = json[JSON_MEMBER_CREDITS];
    for(int i { 0 }; i < creditsJson.Size(); i++) {
        credits_name.push_back({creditsJson[i][JSON_MEMBER_TEXT].GetString(), stod(creditsJson[i][JSON_MEMBER_TIME].GetString())});
    }
    
    vector<pair<string,float>> pictures_name = {};
    if(!json.HasMember(JSON_MEMBER_PICTURES)) LastSupper::AssertUtils::fatalAssert("EndingScriptError\npicturesが存在しません");
    rapidjson::Value& picturesJson = json[JSON_MEMBER_PICTURES];
    for(int i { 0 }; i < picturesJson.Size(); i++) {
        pictures_name.push_back({picturesJson[i][JSON_MEMBER_NAME].GetString(), stod(picturesJson[i][JSON_MEMBER_TIME].GetString())});
    }
    
    if(!json.HasMember(JSON_MEMBER_BACKGROUND)) LastSupper::AssertUtils::fatalAssert("EndingScriptError\nbackgroundが存在しません");
    string backgroundPicture = json[JSON_MEMBER_BACKGROUND].GetString();
    
    if(!json.HasMember(JSON_MEMBER_LAST_TEXT)) LastSupper::AssertUtils::fatalAssert("EndingScriptError\nlastTextが存在しません");
    string lastText = json[JSON_MEMBER_LAST_TEXT].GetString();
    
    if(!json.HasMember(JSON_MEMBER_LAST_PICTURE)) LastSupper::AssertUtils::fatalAssert("EndingScriptError\nlastPictureが存在しません");
    string lastPicture = json[JSON_MEMBER_LAST_PICTURE].GetString();
    
    if(!json.HasMember(JSON_MEMBER_BGM)) LastSupper::AssertUtils::fatalAssert("EndingScriptError\nbgmが存在しません");
    string bgm = json[JSON_MEMBER_BGM].GetString();
    
    // 背景
    Sprite* background {Sprite::createWithSpriteFrameName(backgroundPicture)};
    background->setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    this->addChild(background);
    
    // 文字
    Vector<FiniteTimeAction*> label_acts {};
    label_acts.pushBack(DelayTime::create(15.f));
    int credits_len = credits_name.size();
    
    float x = WINDOW_WIDTH * 4 / 5 - 10;
    float font_size = 28;
    float moving_time = 9.f;
    for (int i = 0; i < credits_len; i++) {
        Label* label {Label::createWithTTF(credits_name[i].first, Resource::Font::MESSAGE, font_size)};
        label->setPosition(x, -1 * label->getContentSize().height / 2);
        label->setColor(Color3B::WHITE);
        this->addChild(label);
        label_acts.pushBack(Sequence::createWithTwoActions(TargetedAction::create(label, DelayTime::create(credits_name[i].second)), TargetedAction::create(label, MoveTo::create(credits_name[i].second < 55 ? moving_time : (moving_time + 1.0), Vec2(label->getPosition().x, label->getContentSize().height / 2 + WINDOW_HEIGHT)))));
    }
    
    // ロゴの位置調整
    float pos = 22.5;
    
    // 制作
    Label* last_label {Label::createWithTTF(lastText, Resource::Font::MESSAGE, font_size)};
    last_label->setPosition(x, -1 * last_label->getContentSize().height / 2);
    last_label->setColor(Color3B::WHITE);
    this->addChild(last_label);
    last_label->runAction(Sequence::createWithTwoActions(DelayTime::create(91.0), MoveTo::create(4.9, Vec2(last_label->getPosition().x, last_label->getContentSize().height * 2.5 + WINDOW_HEIGHT / 2 - pos))));
    
    // ロゴ
    Sprite* logo {Sprite::createWithSpriteFrameName(lastPicture)};
    logo->setPosition(x, -1 * logo->getContentSize().height / 2);
    float scale_logo = 0.5;
    logo->setScale(scale_logo);
    this->addChild(logo);
    logo->runAction(Sequence::createWithTwoActions(DelayTime::create(91.5), MoveTo::create(4.4, Vec2(logo->getPosition().x, WINDOW_HEIGHT/2 - pos))));
    
    // 画像
    vector<Sprite*> pictures {};
    int pic_len = pictures_name.size();
    
    Sprite* sprite {Sprite::create(Resource::SpriteFrame::BASE_PATH + "disp/" + pictures_name[0].first)};
    sprite->setOpacity(0);
    sprite->setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT/2);
    this->addChild(sprite);
    pictures.push_back(sprite);
    
    x = WINDOW_WIDTH / 3 - 20;
    float y = WINDOW_HEIGHT / 2;
    float scale = 0.5;
    for (int i = 1; i < pic_len; i++) {
        Sprite* sprite {Sprite::create(Resource::SpriteFrame::BASE_PATH + "disp/" + pictures_name[i].first)};
        sprite->setOpacity(0);
        sprite->setScale(scale);
        sprite->setPosition(x, y);
        this->addChild(sprite);
        pictures.push_back(sprite);
    }
    
    // 画像の動き
    Vector<FiniteTimeAction*> picture_acts {};
    for (int i = 0; i < pic_len; i++) {
        picture_acts.pushBack(TargetedAction::create(pictures[i], FadeIn::create(1.f)));
        if (i != pic_len - 1) {
            picture_acts.pushBack(TargetedAction::create(pictures[i], DelayTime::create(pictures_name[i].second)));
            picture_acts.pushBack(TargetedAction::create(pictures[i], FadeOut::create(1.f)));
        }
    }
    
    // エディング実行
    SoundManager::getInstance()->playBGM(bgm, false, 2.0);
    this->runAction(Sequence::create(picture_acts));
    this->runAction(Spawn::create(label_acts));
    this->runAction(Sequence::create(DelayTime::create(103), CallFunc::create([this](){
        this->onEndingFinished();
    }), nullptr));
}

// ノーマルエンディングを生成
void EndingScene::createNormalEnding(rapidjson::Value& json)
{
    if(!json.HasMember(JSON_MEMBER_TEXT)) LastSupper::AssertUtils::fatalAssert("EndingScriptError\ntextが存在しません");
    Label* label {Label::createWithTTF(json[JSON_MEMBER_TEXT].GetString(), Resource::Font::SYSTEM, 80)};
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
            PlayerDataManager::getInstance()->setGameClear();
            Director::getInstance()->popScene();
        }), nullptr));
}
