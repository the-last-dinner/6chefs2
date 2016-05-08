//
//  EventListenerKeyboardLayer.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/23.
//
//

#ifndef _EventListenerKeyboardLayer__
#define _EventListenerKeyboardLayer__

#include "Common.h"

// PC版共通キーボード用イベントリスナ
class EventListenerKeyboardLayer : public Layer
{
// クラスメソッド
public:
    CREATE_FUNC(EventListenerKeyboardLayer)
    CREATE_FUNC_WITH_TWO_PARAM(EventListenerKeyboardLayer, const vector<Key>&, const bool)
    
// インスタンス変数
public:
    function<void(const Key&)> onCursorKeyPressed { nullptr };
    function<void()> onEnterKeyPressed { nullptr };
    function<void()> onMenuKeyPressed { nullptr };
    function<void()> onDashKeyPressed { nullptr };
    function<void()> onKeyConfKeyPressed { nullptr };
    function<void(const vector<Key>&)> intervalInputCheck { nullptr };
private:
    EventListenerKeyboard* listenerKeyboard { nullptr };
    map<Key, bool> keyStatus {};
    vector<Key> pressingKeys {};
    float delay {0.05f};        // キーを押した瞬間から初回キー入力確認までの時間
    float interval {0.05f};     // キー入力の確認間隔
    bool paused { false };
    
// インスタンスメソッド
public:
    void onKeyPressed(const EventKeyboard::KeyCode& keyCode);
    void onKeyReleased(const EventKeyboard::KeyCode& keyCode);
    void setEnabled(bool enabled);    // リスナを有効/無効化
    void setInputCheckDelay(float delay);               // キーを押した瞬間から初回キー入力確認までの時間を設定
    void setInputCheckInterval(float interval);         // キー入力の確認間隔を設定
    void releaseKey(const Key& key);  // キーを離すとき
    void releaseKeyAll();  // 全てのキーをリリース状態にリセット
    bool isPressed(const Key& key) const;                          // 指定キーが押し状態か判別
    void setPaused(bool paused);
    vector<Key> getPressedCursorKeys() const;
private:
    EventListenerKeyboardLayer();                                  // コンストラクタ
    ~EventListenerKeyboardLayer();                                 // デストラクタ
    virtual bool init() override;                                  // 初期化
    virtual bool init(const vector<Key>& pressingCursorKeys, const bool isDashPressed);   // 初期化
    void intervalCheck(float duration);                            // キーを押し続けている時
    Key convertKeyCode(const EventKeyboard::KeyCode& keyCode);     // cococs上でのキーコードをゲーム内キーに変換
    void scheduleIntervalCheck();
};

#endif /* defined(_EventListenerKeyboardLayer__) */
