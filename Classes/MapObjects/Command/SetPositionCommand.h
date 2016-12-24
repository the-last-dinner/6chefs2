//
//  SetPositionCommand.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/24.
//
//

#ifndef SetPositionCommand_h
#define SetPositionCommand_h

#include "MapObjects/Command/MapObjectCommand.h"

class SetPositionCommand : public MapObjectCommand
{
// クラスメソッド
public:
    CREATE_FUNC(SetPositionCommand);
    
// インスタンス変数
private:
    Point _gridPosition { Point::ZERO };
    Direction _direction { Direction::NONE };
    
// インスタンスメソッド
private:
    SetPositionCommand();
    ~SetPositionCommand();
    bool init() override;
public:
    void setGridPosition(const Point& gridPosition);
    void setDirection(const Direction& direction);
    
// インターフェース
public:
    virtual bool isExecutable(MapObject* target) const override;
    virtual void execute(MapObject* target) override;
};

#endif /* SetPositionCommand_h */
