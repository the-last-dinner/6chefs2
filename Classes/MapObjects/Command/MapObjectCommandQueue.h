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

class MapObject;

class MapObjectCommandQueue : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC(MapObjectCommandQueue)
    
// インスタンス変数
private:
    Vector<MapObjectCommand*> _commandQueue {};
    MapObjectCommand* _commandInProgress { nullptr };
    
// インスタンスメソッド
private:
    MapObjectCommandQueue();
    virtual ~MapObjectCommandQueue();
    bool init();
    
public:
    // キューにコマンドを詰める
    void push(MapObjectCommand* command);
private:
    // キューの先頭が実行可能か
    bool isFrontExecutable(MapObject* mapObject) const;
    
    // キューの先頭をポップして、そのコマンドを返す
    MapObjectCommand* pop();
    
public:
    // キューをクリア
    void clear();
    
    // キューを確認して実行
    void update(MapObject* mapObject, float delta);
};

#endif /* MapObjectCommandQueue_h */
