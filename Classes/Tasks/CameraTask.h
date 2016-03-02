//
//  CameraTask.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/06.
//
//

#ifndef __LastSupper__CameraTask__
#define __LastSupper__CameraTask__

#include "Tasks/GameTask.h"

class MapObject;
class FollowType;
class TiledMapLayer;

class CameraTask : public GameTask
{
// クラスメソッド
public:
    CREATE_FUNC(CameraTask)

// インスタンス変数
private:
    TiledMapLayer* mapLayer { nullptr };
    MapObject* target { nullptr };
    FollowType* follow { nullptr };
    
// インスタンスメソッド
private:
    CameraTask();
    ~CameraTask();
    bool init();
public:
    void setTarget(MapObject* target);
    void stopFollowing();
    void resumeFollowing();
    void move(const Point& gridPosition, const float duration, function<void()> callback);
    void setCenter(const Point& gridPosition);
    void update(float delta);
};

#endif /* defined(__LastSupper__CameraTask__) */
