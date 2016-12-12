//
//  DungeonSceneEventHandler.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/10/02.
//
//

#ifndef DungeonSceneEventHandler_h
#define DungeonSceneEventHandler_h

#include "define.h"

class DungeonScene;
class Battle;

class DungeonSceneEventHandler : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(DungeonSceneEventHandler, DungeonScene*);
    
// インスタンス変数
private:
    DungeonScene* _scene { nullptr };
    
// インスタンスメソッド
private:
    DungeonSceneEventHandler();
    virtual ~DungeonSceneEventHandler();
    bool init(DungeonScene* scene);
    
public:
    void onLostMainCharacterHP();
    void onBattleStart(Battle* battle);
    void onBattleFinished(Battle* battle);
};

#endif /* DungeonSceneEventHandler_h */
