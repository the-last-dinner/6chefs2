//
//  StepCommand.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/03.
//
//

#ifndef StepCommand_h
#define StepCommand_h

#include "MapObjects/Command/MapObjectCommand.h"

class StepCommand : public MapObjectCommand
{
// クラスメソッド
public:
    CREATE_FUNC(StepCommand);
    
// インスタンス変数
private:
    vector<Direction> _directions {};
    function<void(bool)> _callback { nullptr };
    float _speed { 1.f };
    
// インスタンスメソッド
public:
    void setDirection(const Direction& direction);
    void setDirections(const vector<Direction>& directions);
    void setWalkCallback(function<void(bool)> callback);
    void setSpeed(const float speed);
private:
    StepCommand();
    ~StepCommand();
    bool init() override;
    
// インターフェース
public:
    virtual bool isExecutable(MapObject* target) const override;
    virtual void execute(MapObject* target) override;
};

#endif /* StepCommand_h */
