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
    
// インスタンス変数
private:
    vector<Direction> _directions {};
    function<void()> _onMoved { nullptr };
    float _speed { 1.f };
    
// インスタンスメソッド
private:
    MoveCommand();
    virtual ~MoveCommand();
    virtual bool init() override;
    
public:
    void setDirection(const Direction& direction);
    void setDirections(const vector<Direction>& directions);
    void setMoveCallcback(function<void()> callback);
    void setSpeed(const float speed);
    
// インターフェース
public:
    void execute(MapObject* target, function<void()> callback) override;
};

#endif /* MoveCommand_h */
