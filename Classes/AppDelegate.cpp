#include "AppDelegate.h"
#include "Scenes/StartUpScene.h"

// コンストタクタ
// シングルトンクラスのインスタンスを生成しておく
AppDelegate::AppDelegate()
{
	FUNCLOG
    SoundManager::getInstance();
	TextureManager::getInstance();
	this->init();
}

// デストラクタ
// メモリリークを防ぐため、シングルトンクラスのインスタンスを破棄する
AppDelegate::~AppDelegate() 
{
	FUNCLOG
    CsvDataManager::destroy();
	SoundManager::destory();
    PlayerDataManager::destroy();
	TextureManager::destory();
}

// 初期化関連
void AppDelegate::init()
{
	return;
}

bool AppDelegate::applicationDidFinishLaunching()
{
	FUNCLOG
	auto director = Director::getInstance();
	auto glView = director->getOpenGLView();
	if (!glView) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		glView = GLViewImpl::createWithRect("6人の料理人と隻眼の少女", Rect(0, 0, WINDOW_WIDTH , WINDOW_HEIGHT));
#else	
		glView = GLViewImpl::create("6人の料理人と隻眼の少女");
#endif
		director->setOpenGLView(glView);
	}
	// Tiledのチラつきを防止
	director->setProjection(Director::Projection::_2D);
	director->setDepthTest(false);
	
	// デバッグ表示
    director->setDisplayStats(DebugManager::getInstance()->displayStats());
	
	// フレームレート
	director->setAnimationInterval(1.0 / 60);
	
	// シーンを指定してゲーム開始
	director->runWithScene(StartUpScene::create());
	return true;
}

void AppDelegate::applicationDidEnterBackground()
{
	Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
	Director::getInstance()->startAnimation();
}
