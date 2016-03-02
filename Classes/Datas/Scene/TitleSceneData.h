//
//  TitleSceneData.h
//  LastSupper
//
//  Created by Kohei on 2015/07/04.
//
//

#ifndef __TITLE_SCENE_DATA_H__
#define __TITLE_SCENE_DATA_H__

#include "Datas/Scene/SceneData.h"

class TitleSceneData : public SceneData
{
// クラスメソッド
public:
	CREATE_FUNC(TitleSceneData)

// インスタンスメソッド
private:
	TitleSceneData();
	~TitleSceneData();
	bool init();
};

#endif // __TITLE_SCENE_DATA_H__