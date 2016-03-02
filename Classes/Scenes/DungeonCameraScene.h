//
//  DungeonCameraScene.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/04.
//
//

#ifndef __LastSupper__DungeonCameraScene__
#define __LastSupper__DungeonCameraScene__

#include "Scenes/DungeonScene.h"

#include "Event/GameEvent.h"

class DungeonCameraSceneData;

class DungeonCameraScene : public DungeonScene
{
// 定数
public:
    using EventFinishCallback = function<void()>;
    
// クラスメソッド
public:
    static DungeonCameraScene* create(DungeonCameraSceneData* data, GameEvent* event, EventFinishCallback callback);

// インスタンス変数
private:
    EventFinishCallback callback { nullptr };
    GameEvent* event { nullptr };

// インスタンスメソッド
private:
    DungeonCameraScene();
    ~DungeonCameraScene();
    bool init(DungeonCameraSceneData* data, GameEvent* event, EventFinishCallback callback);
    virtual void onEnter() override;
    virtual void onPreloadFinished(LoadingLayer* loadingLayer) override;
    virtual void onInitEventFinished(LoadingLayer* loadingLayer) override;
    virtual void onAfterInitEventFinished() override;
    void onCameraEventFinished();
    
    DungeonCameraSceneData* getData() const;
};

#endif /* defined(__LastSupper__DungeonCameraScene__) */
