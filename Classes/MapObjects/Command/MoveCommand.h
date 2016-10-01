//
//  MoveCommand.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#ifndef MoveCommand_h
#define MoveCommand_h

#include "MapObjects/Command/MapObjectCommand.h"

class MoveCommand : public MapObjectCommand
{
// クラスメソッド
public:
    CREATE_FUNC(MoveCommand)
    static Vector<MoveCommand*> create(const vector<Direction>& directions,int gridNum, function<void(bool)> cb,float speed);
    static Vector<MoveCommand*> create(const deque<Direction>& directions, function<void(bool)> cb, float speed);
    
// インスタンス変数
private:
    vector<Direction> _directions {};
    function<void(bool)> _callback { nullptr };
    float _speed { 1.f };
    bool _ignoreCollision { false };
    
// インスタンスメソッド
private:
    MoveCommand();
    virtual ~MoveCommand();
    virtual bool init() override;
    
public:
    void setDirection(const Direction& direction);
    void setDirections(const vector<Direction>& directions);
    void setMoveCallback(function<void(bool)> callback);
    void setSpeed(const float speed);
    void setIgnoreCollision(const bool ignore);
    
// インターフェース
public:
    bool isExecutable(MapObject* target) const override;
    void execute(MapObject* target) override;
};

#endif /* MoveCommand_h */
