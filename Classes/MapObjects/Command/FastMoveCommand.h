//
//  FastMoveCommand.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/22.
//
//

#ifndef FastMoveCommand_h
#define FastMoveCommand_h

#include "MapObjects/Command/MapObjectCommand.h"

class FastMoveCommand : public MapObjectCommand
{
// クラスメソッド
public:
    CREATE_FUNC(FastMoveCommand);
    
// インスタンス変数
private:
    Point _gridPosition { Point::ZERO };
    float _delayTime { 0.5f };
    function<void()> _callback { nullptr };
    
// インスタンスメソッド
private:
    FastMoveCommand();
    ~FastMoveCommand();
    virtual bool init() override;
    
public:
    void setGridPosition(const Point& gridPosition);
    void setDelayTime(float delayTime);
    void setCallback(function<void()> callback);
    
// インターフェース
public:
    virtual bool isExecutable(MapObject* target) const override;
    virtual void execute(MapObject* target) override;
};

#endif /* FastMoveCommand_h */
