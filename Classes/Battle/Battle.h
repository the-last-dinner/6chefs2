//
//  Battle.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/23.
//
//

#ifndef Battle_h
#define Battle_h

#include "MapObjects/MapObject.h"

class BattleData;
class EventTask;
class DungeonScene;
class DungeonSceneManager;
class MapObjectList;

class Battle : public Node
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(Battle, BattleData*, DungeonSceneManager*);
    
// インスタンス変数
private:
    BattleData* _data { nullptr };
    MapObject* _mainCharacter { nullptr };
    Vector<MapObject*> _targetObjects {};
    EventTask* _eventTask { nullptr };
    DungeonScene* _scene { nullptr };
    MapObjectList* _objectList { nullptr };
public:
    function<void(Battle*)> _finishCallback {};
    
// インスタンスメソッド
private:
    Battle();
    ~Battle();
    bool init(BattleData* data, DungeonSceneManager* manager);
    void setLostHPCallback(MapObject* target);
    bool isAllTargetDestroyed() const;
    bool isMainCharacterDestroyed() const;
    void onFinish();
    
public:
    void start();
    Vector<MapObject*> getTargetObjects() const;
    void update(float delta) override;
};

#endif /* Battle_hpp */
