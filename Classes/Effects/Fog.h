//
//  Fog.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/19.
//
//

#ifndef Fog_h
#define Fog_h

#include "define.h"

class Fog : public Node
{
// 定数
private:
    static const float DURATION;
    static const string IMG_FILE_NAME;
    
// クラスメソッド
public:
    CREATE_FUNC(Fog)
    
// インスタンスメソッド
private:
    Fog();
    ~Fog();
    bool init();
    void moveSprites(Sprite* upper, Sprite* bottom);
    void onMoveAnimationFinished(Sprite* upper, Sprite* bottom);
};

#endif /* Fog_h */
