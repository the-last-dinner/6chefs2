#include "AppDelegate.h"

#include "Managers/KeyconfigManager.h"
#include "Managers/EventListenerKeyboardManager.h"

#include "Scenes/StartUpScene.h"

// コンストタクタ
AppDelegate::AppDelegate()
{
	FUNCLOG

    SoundManager::getInstance();
	TextureManager::getInstance();
    KeyconfigManager::getInstance();
}

// デストラクタ
AppDelegate::~AppDelegate() 
{
	FUNCLOG
    
    CsvDataManager::destroy();
	SoundManager::destory();
    PlayerDataManager::destroy();
	TextureManager::destory();
    EventListenerKeyboardManager::destroy();
}

bool AppDelegate::applicationDidFinishLaunching()
{
	FUNCLOG
	auto director = Director::getInstance();
	auto glView = director->getOpenGLView();
	if (!glView) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
		glView = GLViewImpl::createWithRect("6人の料理人と隻眼の少女2", Rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
        glView->setFrameSize(WINDOW_WIDTH * WINDOW_SIZE_RATIO_MIDDLE, WINDOW_HEIGHT * WINDOW_SIZE_RATIO_MIDDLE);
        glView->setDesignResolutionSize(WINDOW_WIDTH, WINDOW_HEIGHT, ResolutionPolicy::NO_BORDER);
#else	
		glView = GLViewImpl::create("6人の料理人と隻眼の少女2");
#endif
		director->setOpenGLView(glView);
	}
	// Tiledのチラつきを防止
	director->setProjection(Director::Projection::_2D);
	director->setDepthTest(false);
	
	// デバッグ表示
    director->setDisplayStats(ConfigDataManager::getInstance()->getDebugConfigData()->getBoolValue(DebugConfigData::STATS));
	
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
