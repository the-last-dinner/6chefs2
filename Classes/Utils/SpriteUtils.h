//
//  SpriteUtils.hpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/07.
//
//

#ifndef SpriteUtils_h
#define SpriteUtils_h

#include "define.h"

namespace SpriteUtils{
    // 四角形構造体
    struct Square
    {
    private:
        float rate_width = WINDOW_WIDTH / 100;
        float rate_height = WINDOW_HEIGHT / 100;
        float minX {0};
        float minY {0};
        float maxX {0};
        float maxY {0};
        float width {0};
        float height {0};
    public:
        Square(const float minX, const float minY, const float maxX, const float maxY):minX(minX * rate_width), minY(minY * rate_height), maxX(maxX * rate_width), maxY(maxY * rate_height){
            width = this->maxX - this->minX;
            height = this->maxY - this->minY;
        };
        Square(){};
        // getter
        const float getMinX(){return minX;};
        const float getMinY(){return minY;};
        const float getMaxX(){return maxX;};
        const float getMaxY(){return maxY;};
        const float getWidth(){return width;};
        const float getHeight(){return height;};
    };
    // 空白構造体
    struct Margin
    {
    private:
        float left {0};
        float right {0};
        float top {0};
        float bottom {0};
    public:
        // 上,右,下,左
        Margin(const float top, const float right, const float bottom, const float left):top(top), right(right), bottom(bottom), left(left){};
        // 上,右左,下
        Margin(const float top, const float right_left, const float bottom):top(top), right(right_left), bottom(bottom), left(right_left){};
        // 上下,右左
        Margin(const float top_bottom, const float right_left):top(top_bottom), right(right_left), bottom(top_bottom), left(right_left){};
        // 上右左下
        Margin(const float margin):top(margin), right(margin), left(margin), bottom(margin){};
        // 空
        Margin(){};
        // getter
        float getLeft(){return left;};
        float getRight(){return right;};
        float getTop(){return top;};
        float getBottom(){return bottom;};
    };
    // 四角形のスプライトを得る
    Sprite* getSquareSprite(Square& range, Margin& margin);
    Sprite* getSquareSprite(Square& range, const float margin);
    
}

#endif /* SpriteUtils_h */
