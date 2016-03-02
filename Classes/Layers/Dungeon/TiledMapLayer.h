//
//  TiledMapLayer.h
//  LastSupper
//
//  Created by Kohei on 2015/06/28.
//
//

#ifndef __TILED_MAP_LAYER_H__
#define __TILED_MAP_LAYER_H__

#include "Common.h"

class Enemy;
class MapObject;
class MapObjectList;
class Party;

class TiledMapLayer : public Layer
{
// クラスメソッド
public:
	CREATE_FUNC_WITH_PARAM(TiledMapLayer, const Location&)

// インスタンス変数
private:
    TMXTiledMap* tiledMap { nullptr };    // マップ背景
    MapObjectList* objectList { nullptr };              // マップオブジェクトのリスト
    
// インスタンスメソッド
private:
	TiledMapLayer();
	~TiledMapLayer();
	bool init(const Location&);
    void orderLayers();
	
public:
    MapObjectList* getMapObjectList() const;
    Size getMapSize() const;
    // Layer
    void hideLayer(const string& layerName);
    void swingLayer(const string& layerName);
    void stopLayerActions();
    // MapObject
    void setParty(Party* party);
    void addEnemy(Enemy* enemy);
    void addMapObject(MapObject* mapObject, bool addingToList = true);
    void setZOrderByPosition(MapObject* mapObject);
    void setMapObjectPosition(MapObject* mapObject);
};

#endif // __TILED_MAP_LAYER_H__
