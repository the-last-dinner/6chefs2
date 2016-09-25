//
//  MapObjectCommandQueue.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#ifndef MapObjectCommandQueue_h
#define MapObjectCommandQueue_h

#include "MapObjects/Command/MapObjectCommand.h"

class MapObjectCommandQueue : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC(MapObjectCommandQueue)
    
// インスタンス変数
private:
    Vector<MapObjectCommand*> _commandQueue {};
    bool _isInProgress { false };
    
// インスタンスメソッド
private:
    MapObjectCommandQueue();
    virtual ~MapObjectCommandQueue();
    bool init();
    
public:
    void push(MapObjectCommand* command);
    MapObjectCommand* pop();
    void clear();
};

#endif /* MapObjectCommandQueue_h */
