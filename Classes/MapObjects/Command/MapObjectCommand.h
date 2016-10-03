//
//  MapObjectCommand.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#ifndef MapObjectCommand_h
#define MapObjectCommand_h

#include "define.h"

class MapObject;

class MapObjectCommand : public Ref
{
// インスタンス変数
private:
    bool _isDone { false };
    
// インスタンスメソッド
protected:
    MapObjectCommand();
    virtual ~MapObjectCommand();
    virtual bool init();
    void setDone();
public:
    bool isDone() const;

// インターフェース
public:
    virtual bool isExecutable(MapObject* target) const = 0;
    virtual void execute(MapObject* target) = 0;
    
};

#endif /* MapObjectCommand_h */
