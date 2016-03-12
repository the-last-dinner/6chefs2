//
//  GameTask.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/16.
//
//

#ifndef __LastSupper__GameTask__
#define __LastSupper__GameTask__

#include "Common.h"

// タスククラスの基底
class GameTask : public Node
{
// インスタンス変数
protected:
    
// インスタンスメソッド
protected:
    GameTask();
    virtual ~GameTask();
    virtual bool init();
};

#endif /* defined(__LastSupper__GameTask__) */
