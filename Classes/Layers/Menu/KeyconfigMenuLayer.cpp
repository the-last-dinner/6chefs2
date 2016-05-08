//
//  KeyconfigMenuLayer.cpp
//  6chef2
//
//  Created by Kohei Asami on 2016/03/12.
//
//

#include "Layers/Menu/KeyconfigMenuLayer.h"

#include "Managers/KeyconfigManager.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// 定数
const map<KeyconfigMenuLayer::MenuType, int> KeyconfigMenuLayer::idNums
{
    {MenuType::CURSOR, etoi(KeyconfigManager::CursorKeyType::SIZE)},
    {MenuType::DASH, etoi(KeyconfigManager::DashKeyType::SIZE)},
};

const int KeyconfigMenuLayer::RIGHT_ARROW_TAG {1};
const int KeyconfigMenuLayer::LEFT_ARROW_TAG {2};

// コンストラクタ
KeyconfigMenuLayer::KeyconfigMenuLayer() { FUNCLOG };

// デストラクタ
KeyconfigMenuLayer::~KeyconfigMenuLayer()
{
    FUNCLOG
    
    if(this->onClose) this->onClose(this);
};

// 初期化
bool KeyconfigMenuLayer::init()
{
    if(!MenuLayer::init(1, etoi(MenuType::SIZE))) return false;
    
    this->listenerKeyboard->onKeyConfKeyPressed = CC_CALLBACK_0(KeyconfigMenuLayer::close, this);
    this->setLocalZOrder(Priority::KEY_CONFIG);
    
    Sprite* cover {Sprite::create()};
    cover->setTextureRect(Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    cover->setColor(Color3B::WHITE);
    cover->setPosition(WINDOW_CENTER);
    cover->setBlendFunc({GL_ZERO, GL_SRC_COLOR});
    this->addChild(cover);
    this->cover = cover;
    
    Node* container { Node::create() };
    container->setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 75);
    container->setContentSize(Size(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
    container->setCascadeOpacityEnabled(true);
    this->addChild(container);
    this->container = container;
    
    float width {container->getContentSize().width};
    float unitHeight {container->getContentSize().height / 5};
    
    for(int i {0}; i < etoi(MenuType::SIZE); i++)
    {
        Sprite* frame { this->createFrame(Size(width, unitHeight), i) };
        frame->setPosition(this->calcItemPosition(frame, i));
        frame->setTag(i);
        frame->setCascadeOpacityEnabled(true);
        container->addChild(frame);
        this->menuObjects.push_back(frame);
    }
    
    string fontPath {Resource::Font::CONFIG};
    float centerMargin { 20 };
    
    Node* cursorConfigFrame { this->menuObjects.at(etoi(MenuType::CURSOR)) };
    Label* cursorConfigLabel { Label::createWithTTF("移動キー", fontPath, cursorConfigFrame->getContentSize().height * 0.4) };
    cursorConfigLabel->setPosition(this->calcConfigNamePosition(cursorConfigLabel, cursorConfigFrame, centerMargin));
    cursorConfigFrame->addChild(cursorConfigLabel);
    
    Node* dashConfigFrame { this->menuObjects.at(etoi(MenuType::DASH)) };
    Label* dashConfigLabel { Label::createWithTTF("ダッシュキー", fontPath, dashConfigFrame->getContentSize().height * 0.4) };
    dashConfigLabel->setPosition(this->calcConfigNamePosition(dashConfigLabel, dashConfigFrame, centerMargin));
    dashConfigFrame->addChild(dashConfigLabel);
    
    Sprite* saveFrame { dynamic_cast<Sprite*>(this->menuObjects.at(etoi(MenuType::SAVE))) };
    saveFrame->setTextureRect(Rect(0, 0, saveFrame->getContentSize().width, saveFrame->getContentSize().height / 2));
    saveFrame->setPosition(saveFrame->getPosition().x, saveFrame->getPosition().y + 10);
    Label* saveLabel { Label::createWithTTF("適用", fontPath, saveFrame->getContentSize().height * 0.6) };
    saveLabel->setPosition(saveFrame->getContentSize() / 2);
    saveFrame->addChild(saveLabel);
    
    Sprite* cancelFrame { dynamic_cast<Sprite*>(this->menuObjects.at(etoi(MenuType::CANCEL))) };
    cancelFrame->setTextureRect(Rect(0, 0, cancelFrame->getContentSize().width, cancelFrame->getContentSize().height / 2));
    cancelFrame->setPosition(cancelFrame->getPosition().x, cancelFrame->getPosition().y + 35);
    Label* cancelLabel { Label::createWithTTF("もどる", fontPath, cancelFrame->getContentSize().height * 0.6) };
    cancelLabel->setPosition(cancelFrame->getContentSize() / 2);
    cancelFrame->addChild(cancelLabel);
    
    Label* menuTitle { Label::createWithTTF("キーコンフィグ", fontPath, unitHeight * 0.5) };
    menuTitle->setPosition(-width / 2 + menuTitle->getContentSize().width / 2, cursorConfigFrame->getPosition().y + cursorConfigFrame->getContentSize().height);
    container->addChild(menuTitle);
    
    this->configIdxs[MenuType::CURSOR] = etoi(KeyconfigManager::getInstance()->getCursorKeyType());
    this->configIdxs[MenuType::DASH] = etoi(KeyconfigManager::getInstance()->getDashKeyType());
    this->setConfigNameLabel(MenuType::CURSOR, etoi(KeyconfigManager::getInstance()->getCursorKeyType()));
    this->setConfigNameLabel(MenuType::DASH, etoi(KeyconfigManager::getInstance()->getDashKeyType()));
    
    this->arrows = Node::create();
    this->arrows->setCascadeOpacityEnabled(true);
    this->arrows->setVisible(false);
    container->addChild(this->arrows);
    
    DrawNode* arrow {DrawNode::create()};
    arrow->drawTriangle(Vec2::ZERO, Vec2(10, 5), Vec2(10, -5), Color4F::GRAY);
    arrow->setPosition(0, 0);
    arrow->setTag(LEFT_ARROW_TAG);
    this->arrows->addChild(arrow);
    
    arrow = DrawNode::create();
    arrow->drawTriangle(Vec2::ZERO, Vec2(-10, 5), Vec2(-10, -5), Color4F::GRAY);
    arrow->setPosition(width / 2 - 10, 0);
    arrow->setTag(RIGHT_ARROW_TAG);
    this->arrows->addChild(arrow);
    
    this->showWithAnimation([this](KeyconfigMenuLayer* layer){ this->onIndexChanged(this->getSelectedIndex(), false); this->listenerKeyboard->setEnabled(true); });
    
    return true;
}

// 方向キーを押した時
void KeyconfigMenuLayer::onCursorKeyPressed(const Key& key)
{
    MenuLayer::onCursorKeyPressed(key);
    
    MenuType type { static_cast<MenuType>(this->getSelectedIndex()) };
    
    if(!(key == Key::RIGHT || key == Key::LEFT)) return;
    if(type == MenuType::SAVE || type == MenuType::CANCEL) return;
    
    switch (key) {
        case Key::RIGHT:
            this->configIdxs[type] = (this->configIdxs[type] + 1) % idNums.at(type);
            break;
            
        case Key::LEFT:
            this->configIdxs[type] = (this->configIdxs[type] == 0)? idNums.at(type) - 1 : (this->configIdxs[type] - 1) % idNums.at(type);
            break;
            
        default:
            break;
    }
    
    this->onConfigIdxChanged(type, this->configIdxs[type]);
}

// カーソルを移動させた時
void KeyconfigMenuLayer::onIndexChanged(int newIdx, bool sound)
{
    if(sound) SoundManager::getInstance()->playSE(Resource::SE::CURSOR_MOVE);
    
    for(Node* obj : this->menuObjects)
    {
        obj->stopAllActions();
        obj->setColor(this->getFrameColor(obj->getTag()));
    }
    
    Node* focusedItem { this->menuObjects.at(newIdx) };
    
    focusedItem->setColor(Color3B(41, 128, 185));
    focusedItem->runAction(RepeatForever::create(Sequence::create(TintTo::create(1.0, Color3B(41, 128, 185)), TintTo::create(1.0, Color3B(52, 152, 219)), nullptr)));
    
    this->arrows->setPosition(focusedItem->getPosition());
    
    MenuType type {static_cast<MenuType>(newIdx)};
    this->arrows->setVisible(!(type == MenuType::SAVE || type == MenuType::CANCEL));
}

// 決定キーを押した時
void KeyconfigMenuLayer::onEnterKeyPressed(int idx)
{
    MenuType type { static_cast<MenuType>(idx) };
    switch (type) {
        case MenuType::SAVE:
            // なぜかKeyConfigManagerのCursorKeyTypeとかを変数に代入できない！後で検討
            KeyconfigManager::getInstance()->setCursorKey(static_cast<KeyconfigManager::CursorKeyType>(this->configIdxs[MenuType::CURSOR]));
            KeyconfigManager::getInstance()->setDashKey(static_cast<KeyconfigManager::DashKeyType>(this->configIdxs[MenuType::DASH]));
            PlayerDataManager::getInstance()->getGlobalData()->saveKeyConfig
            (
                static_cast<KeyconfigManager::CursorKeyType>(this->configIdxs[MenuType::CURSOR]),
                static_cast<KeyconfigManager::DashKeyType>(this->configIdxs[MenuType::DASH])
            );
            this->close();
            break;
        case MenuType::CANCEL:
            this->close();
            break;
        default:
            break;
    }
}

// メニューキーを押した時
void KeyconfigMenuLayer::onMenuKeyPressed() {}

// 表示
void KeyconfigMenuLayer::show() {}

// 非表示
void KeyconfigMenuLayer::hide() {}

// 設定内容を変更した時
void KeyconfigMenuLayer::onConfigIdxChanged(MenuType type, int newIdx)
{
    this->setConfigNameLabel(type, newIdx);
}

// 閉じる
void KeyconfigMenuLayer::close()
{
    this->hideWithAnimation(this->onClose);
}

// アニメーションしながら表示
void KeyconfigMenuLayer::showWithAnimation(AnimationCallback callback)
{
    this->cover->runAction(TintTo::create(0.3f, Color3B(100, 100, 100)));
    
    Point toPosition { this->container->getPosition() };
    this->container->setPosition(toPosition - Vec2(0, 30));
    this->container->setOpacity(0);
    
    this->container->runAction(Sequence::create(Spawn::createWithTwoActions(EaseCubicActionOut::create(MoveTo::create(0.4f, toPosition)), FadeIn::create(0.4f)), CallFunc::create([this, callback]{if(callback) callback(this);}), nullptr));
}

// アニメーションしながら非表示
void KeyconfigMenuLayer::hideWithAnimation(AnimationCallback callback)
{
    this->arrows->setVisible(false);
    this->listenerKeyboard->setEnabled(false);
    this->cover->runAction(TintTo::create(0.3f, Color3B::WHITE));
    this->container->runAction(Sequence::createWithTwoActions(FadeOut::create(0.2f), CallFunc::create([this, callback]{if(callback) callback(this);})));
}

// 枠を生成
Sprite* KeyconfigMenuLayer::createFrame(const Size& size, int idx) const
{
    Sprite* frame {Sprite::create()};
    frame->setColor(this->getFrameColor(idx));
    frame->setTextureRect(Rect(0, 0, size.width, size.height));
    frame->setOpacity(200);
    
    return frame;
}

// 設定名をセット
void KeyconfigMenuLayer::setConfigNameLabel(const MenuType type, int idx)
{
    string configNameStr {};
    switch (type) {
        case MenuType::CURSOR:
            configNameStr = KeyconfigManager::getInstance()->typeToDispName(static_cast<KeyconfigManager::CursorKeyType>(idx));
            break;
            
        case MenuType::DASH:
            configNameStr = KeyconfigManager::getInstance()->typeToDispName(static_cast<KeyconfigManager::DashKeyType>(idx));
            break;
            
        default:
            break;
    }
    
    int labelTag {999};
    Node* frame { this->menuObjects.at(etoi(type)) };
    frame->removeChildByTag(labelTag);
    Label* configNameLabel { Label::createWithTTF(configNameStr, Resource::Font::CONFIG, frame->getContentSize().height * 0.4) };
    configNameLabel->setPosition(frame->getContentSize().width / 2 + configNameLabel->getContentSize().width / 2 + 20, frame->getContentSize().height / 2);
    configNameLabel->setTag(labelTag);
    frame->addChild(configNameLabel);
}

// 枠の色を取得
Color3B KeyconfigMenuLayer::getFrameColor(int idx) const
{
    return (idx % 2 == 0)? Color3B::BLACK : Color3B(30, 30, 30);
}

// メニュー項目の座標を計算
Point KeyconfigMenuLayer::calcItemPosition(Sprite* item, int idx) const
{
    int itemNumFix { etoi(MenuType::SIZE) / 2 };
    float evenFix { etoi(MenuType::SIZE) % 2 == 0 ? 0.5 : 0 };
    
    return Point(0, (itemNumFix + evenFix) * (itemNumFix - idx) * item->getContentSize().height * 0.4f);
}

// メニュー項目名の座標を計算
Point KeyconfigMenuLayer::calcConfigNamePosition(Label* label, Node* frame, float centerMargin) const
{
    return Point(frame->getContentSize().width / 2 - label->getContentSize().width / 2 - centerMargin, frame->getContentSize().height / 2);
}
