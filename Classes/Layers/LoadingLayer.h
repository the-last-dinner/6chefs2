//
//  LoadingLayer.h
//  LastSupper
//
//  Created by Kohei on 2015/07/04.
//
//

#ifndef __LOADING_LAYER_H__
#define __LOADING_LAYER_H__

#include "Common.h"

class LoadingLayer : public Layer
{
// クラスメソッド
public:
    CREATE_FUNC(LoadingLayer);
	CREATE_FUNC_WITH_PARAM(LoadingLayer, const Color4B&);
    
// インスタンス変数
private:
	
// インスタンスメソッド
private:
	LoadingLayer();
	~LoadingLayer();
    virtual bool init();
    virtual bool init(const Color4B& bgColor);
public:
	void onLoadFinished();
};

#endif // __LOADING_LAYER_H__
