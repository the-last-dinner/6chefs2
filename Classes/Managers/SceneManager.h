//
//  SceneManager.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/05/14.
//
//

#ifndef SceneManager_h
#define SceneManager_h

#include "define.h"

class RootScene;

class SceneManager
{
// クラスメソッド
public:
    static SceneManager* getInstance();
    static void  destroy();
    
// インスタンス変数
private:
    Scene* _runningScene         { nullptr };
    Vector<Scene*> _sceneStack   {};

// インスタンスメソッド
private:
    SceneManager();                                         //コンストラクタ
    ~SceneManager();                                        //デストラクタ
    SceneManager(const SceneManager& other){};              //コピーコンストラクタ
    SceneManager& operator = (const SceneManager& other);   //代入演算子
public:
    RootScene* getRootScene() const;
    Scene* getRunningScene() const;
public:
    void runWithScene(Scene* scene);
    void replaceScene(Scene* scene);
    void pushScene(Scene* scene);
    void popScene();
private:
    void setNextScene(Scene* scene);
};

#endif /* SceneManager_h */
