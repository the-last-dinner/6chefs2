//
//  SpriteUtils.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/11/07.
//
//

#include "Utils/SpriteUtils.h"

// 四角形のスプライトを生成
Sprite* SpriteUtils::getSquareSprite(SpriteUtils::Square &range, SpriteUtils::Margin &margin)
{
    Sprite* square = Sprite::create();
    float width = range.getWidth() - margin.getLeft() - margin.getRight();
    float height = range.getHeight() - margin.getTop() - margin.getBottom();
    square->setTextureRect(Rect(0, 0, width, height));
    square->setPosition((width / 2) + range.getMinX() + margin.getLeft(), (height / 2) + range.getMinY() + margin.getBottom());
    return square;
}

// 四角形のスプライトをマージン一括していで生成
Sprite* SpriteUtils::getSquareSprite(SpriteUtils::Square &range, const float margin)
{
    Margin strc = Margin(margin);
    return SpriteUtils::getSquareSprite(range, strc);
}