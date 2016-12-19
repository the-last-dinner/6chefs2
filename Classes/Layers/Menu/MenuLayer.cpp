//
//  MenuLayer.cpp
//  LastSupper
//
//  Created by Kohei on 2015/07/05.
//
//

#include "Layers/Menu/MenuLayer.h"

#include "Layers/EventListener/EventListenerKeyboardLayer.h"

// コンストラクタ
MenuLayer::MenuLayer(){FUNCLOG}

// デストラクタ
MenuLayer::~MenuLayer(){FUNCLOG}

// 初期化 (ページ数指定有)
bool MenuLayer::init(const Point& pageSize, const int itemCount, Node* basePanel)
{
    int sizeX = itemCount < pageSize.x ? itemCount : pageSize.x;
    int sizeY = itemCount < pageSize.x * pageSize.y ? floor(itemCount / pageSize.x) + 1 : pageSize.y;
    this->pageCount = floor(abs(itemCount - 1) / (pageSize.x * pageSize.y)) + 1;
    
    // pagePanel生成
    for (int i = 0; i < this->pageCount; i++)
    {
        Sprite* pagePanel = Sprite::create();
        pagePanel->setTextureRect(Rect(0,0,basePanel->getContentSize().width, basePanel->getContentSize().height));
        pagePanel->setPosition(pagePanel->getContentSize().width/2, pagePanel->getContentSize().height/2);
        pagePanel->setOpacity(0);
        pagePanel->setName("pagePanel");
        CC_SAFE_RETAIN(pagePanel);
        this->pagePanels.push_back(pagePanel);
    }
    this->basePanel = basePanel;
    
    return this->init(sizeX, sizeY);
}

// 初期化
bool MenuLayer::init(const Point& index, const Size& size)
{
	this->indexX = index.x;
	this->indexY = index.y;
	
	return this->init(size.width, size.height);
}

// 初期化
bool MenuLayer::init(int sizeX, int sizeY)
{
	FUNCLOG
	if(!Layer::init()) return false;
	
	// メニューの項目数を設定
	this->sizeX = sizeX;
	this->sizeY = sizeY;
    
    EventListenerKeyboardLayer* listenerKeyboard { EventListenerKeyboardLayer::create() };
    this->addChild(listenerKeyboard);
    listenerKeyboard->onCursorKeyPressed = CC_CALLBACK_1(MenuLayer::onCursorKeyPressed, this);
    listenerKeyboard->onEnterKeyPressed = CC_CALLBACK_0(MenuLayer::onEnterPressed, this);
    listenerKeyboard->onMenuKeyPressed = CC_CALLBACK_0(MenuLayer::onMenuKeyPressed, this);
    listenerKeyboard->intervalInputCheck = CC_CALLBACK_1(MenuLayer::intervalInputCheck, this);
    listenerKeyboard->setInputCheckDelay(0.3f);
    listenerKeyboard->setInputCheckInterval(0.1f);
    listenerKeyboard->setEnabled(false);
    this->listenerKeyboard = listenerKeyboard;

	return true;
}

// 方向キーを押した時
void MenuLayer::onCursorKeyPressed(const Key& key)
{
    // カーソルが無効な場合は何もしない
    if(!this->cursorEnabled) return;
    
    // 現在のインデックスを保存
    int old_indexX = this->indexX;
    int old_indexY = this->indexY;
    
    // インデックスの変更
    switch(key)
    {
        case Key::UP:
            if (this->pageCount > 1 && indexY == 0)
            {
                this->page = this->page == 0 ? this->pageCount - 1 : this->page - 1;
                this->onPageChanged(this->page);
            }
            if(sizeY >= 2) this->indexY = (indexY == 0) ? indexY = sizeY - 1 : (indexY - 1) % sizeY;
            break;
            
        case Key::DOWN:
            if (this->pageCount > 1 && indexY + 1 == sizeY)
            {
                this->page = (this->page + 1) % this->pageCount;
                this->onPageChanged(this->page);
            }
            if(sizeY >= 2) this->indexY = (indexY + 1) % sizeY;
            // ページチェック
            if (this->calcSelectedIndex() > this->menuObjects.size() - 1)
            {
                if (this->calcSelectedIndex() - sizeX < this->page * sizeX * sizeY)
                {
                    // 飛び先のページに要素が存在しないとき
                    this->indexX = (this->menuObjects.size() - 1) % sizeX;
                }
                else
                {
                    // 飛び先のページの同じindexXのところへ飛ぶ
                    this->page = 0;
                    this->indexY = 0;
                }
                this->onPageChanged(this->page);
                
            }
            break;
            
        case Key::LEFT:
            if(sizeX >= 2) this->indexX = (indexX == 0)? indexX = sizeX - 1 : (indexX - 1) % sizeX;
            break;
            
        case Key::RIGHT:
            if(sizeX >= 2) this->indexX = (indexX + 1) % sizeX;
            break;
            
        default:
            break;
    }
    
    // ヴィジュアルの更新
    bool sound =  (this->indexX == old_indexX && this->indexY == old_indexY) ? false : true;
    this->onIndexChanged(this->getSelectedIndex(), sound);
    //cout << "page>>" << this->page << endl <<"indexX>>" << indexX << endl << "indexY>>" << indexY << endl;
}

// 決定キーを押した時
void MenuLayer::onEnterPressed()
{
    this->onEnterKeyPressed(this->getSelectedIndex());
}

// メニューキーを押した時
void MenuLayer::onMenuKeyPressed()
{
    int size = this->pagePanels.size();
    for (int i = 0; i < size; i++)
    {
        CC_SAFE_RELEASE(this->pagePanels[i]);
    }
}

// 現在選ばれているメニューのINDEXを取得(現時点では横優先配置の場合のみ)
int MenuLayer::getSelectedIndex()
{
    int selected = this->calcSelectedIndex();
    int maxIndex = this->menuObjects.size() - 1;
    if (selected > maxIndex){
        if (sizeY >= 2) this->indexY = ((int)floor((maxIndex + 1) / sizeX)) % sizeY;
        selected = this->calcSelectedIndex();
        if (selected > maxIndex)
        {
            if (selected - sizeX < this->page * sizeX * sizeY)
            {
                selected = maxIndex;
                if (sizeX >= 2) this->indexX = (maxIndex + 1) % sizeX;
                if (sizeY >= 2) this->indexY = ((int)floor((maxIndex + 1) / sizeX)) % sizeY;
            }
            else
            {
                indexY--;
                selected -= sizeX;
            }
        }
    }
    return selected;
}

int MenuLayer::calcSelectedIndex()
{
    return this->sizeX * this->indexY + this->indexX + this->page * this->sizeX * this->sizeY;
}

// メニューサイズの横方向を取得
int MenuLayer::getMenuSizeX() const
{return this->sizeX;}

// メニューサイズの縦方向を取得
int MenuLayer::getMenuSizeY() const
{return this->sizeY;}

// メニューのインデックスをデフォルトセット
void MenuLayer::setSelectedIndex(const int index)
{
    this->indexX = index % this->sizeX;
    this->indexY = (index / this->sizeX) % this->sizeY;
}

// カーソルの有効無効を設定
void MenuLayer::setCursorEnable(const bool enable)
{
    this->cursorEnabled = enable;
}

void MenuLayer::intervalInputCheck(const vector<Key>& keys)
{
    if(keys.empty()) return;
    
    this->onCursorKeyPressed(keys.back());
}

// ページサイズを取得
int MenuLayer::getPageSize(){return this->pageCount;}

// ページが変わった時
void MenuLayer::onPageChanged(const int page)
{
    FUNCLOG
    //　ページ処理
    this->basePanel->removeChildByName("pagePanel");
    this->basePanel->addChild(this->pagePanels[page]);
    Size listContentSize = this->basePanel->getContentSize();
    int fontSize = 16;
    
    // ページカウンター
    if (this->basePanel->getChildByName("counter"))
    {
        this->basePanel->removeChildByName("counter");
    }
    if (this->pageCount > 1)
    {
        Label* counter = Label::createWithTTF(to_string(page+1) + "/" + to_string(this->pageCount), "fonts/cinecaption2.28.ttf", fontSize * 1.25);
        counter->setPosition(counter->getContentSize().width/2 + 5 , counter->getContentSize().height/2 + 5);
        counter->setColor(Color3B::WHITE);
        counter->setName("counter");
        this->basePanel->addChild(counter);
    }
    
    // 下への矢印
    if (this->basePanel->getChildByName("downPager"))
    {
        this->basePanel->removeChildByName("downPager");
    }
    if (page + 1 != this->pageCount)
    {
        Label* downPager = Label::createWithTTF("▼", "fonts/cinecaption2.28.ttf", fontSize);
        downPager->setPosition(listContentSize.width/2 , fontSize * 0.75);
        downPager->setColor(Color3B::WHITE);
        downPager->setName("downPager");
        this->basePanel->addChild(downPager);
        
        // アクション生成
        this->moveUpDown(downPager);
    }
    
    // 上への矢印
    if (this->basePanel->getChildByName("upPager"))
    {
        this->basePanel->removeChildByName("upPager");
    }
    if (page != 0)
    {
        Label* upPager = Label::createWithTTF("▲", "fonts/cinecaption2.28.ttf", 16);
        upPager->setPosition(listContentSize.width/2 , listContentSize.height - fontSize * 0.75);
        upPager->setColor(Color3B::WHITE);
        upPager->setName("upPager");
        this->basePanel->addChild(upPager);
        
        // アクション生成
        this->moveUpDown(upPager);
    }
}

// 上下に反復移動
void MenuLayer::moveUpDown(cocos2d::Node* target)
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
