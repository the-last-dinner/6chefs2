//
//  DungeonSceneData.h
//  LastSupper
//
//  Created by Kohei on 2015/06/23.
//
//

#ifndef __DUNGEON_SCENE_DATA_H__
#define __DUNGEON_SCENE_DATA_H__

#include "Datas/Scene/SceneData.h"

class DungeonSceneData : public SceneData
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(DungeonSceneData, const Location&)
    
// インスタンス変数
private:
    int initEventId { etoi(EventID::UNDIFINED) };
    Location location {};
	
// インスタンスメソッド
protected:
	DungeonSceneData();
	~DungeonSceneData();
    bool init(const Location& location);
public:
    void setInitialEventId(const int eventId);
    int getInitialEventId() const;
    Location getInitialLocation() const;
};

#endif // __DUNGEON_SCENE_DATA_H__
