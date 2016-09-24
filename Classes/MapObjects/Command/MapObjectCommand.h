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
// インスタンスメソッド
protected:
    MapObjectCommand();
    virtual ~MapObjectCommand();
    virtual bool init();
    
public:
    virtual void execute(MapObject* target, function<void()> callback) = 0;
};

#endif /* MapObjectCommand_h */
