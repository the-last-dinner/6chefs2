//
//  StartUpSceneData.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/13.
//
//

#include "Datas/Scene/StartUpSceneData.h"


bool StartUpSceneData::init()
{
    this->textureFilePaths =
    {
        Resource::SpriteFrame::ANIMATION,
        Resource::SpriteFrame::STARTUP,
        Resource::SpriteFrame::TITLE,
        Resource::SpriteFrame::FRAME,
        Resource::SpriteFrame::MENU_PANELS,
        Resource::SpriteFrame::ICON,
        Resource::SpriteFrame::UI,
        Resource::SpriteFrame::MAP_OBJECT,
        Resource::SpriteFrame::EFFECT,
        Resource::SpriteFrame::Character::MAGOICHI,
        Resource::SpriteFrame::Character::DAIGORO,
        Resource::SpriteFrame::Character::DANDAN,
        Resource::SpriteFrame::Character::NADESHIKO,
        Resource::SpriteFrame::Character::MANAKA,
        Resource::SpriteFrame::Character::RANMARU,
        Resource::SpriteFrame::Character::SHITSUJI,
        Resource::SpriteFrame::Character::YUKI,
        Resource::SpriteFrame::Character::FUJIN,
        Resource::SpriteFrame::Character::KNIFE,
        Resource::SpriteFrame::Character::TAIHO,
        Resource::SpriteFrame::Character::T_FUJIN,
        Resource::SpriteFrame::Character::W_MANAKA,
        Resource::SpriteFrame::Character::W_DANDAN,
        Resource::SpriteFrame::Character::O_RANMARU,
        Resource::SpriteFrame::Character::S_RANMARU,
        Resource::SpriteFrame::Character::D_DAIGORO,
        Resource::SpriteFrame::Character::KYOJIN_1,
        Resource::SpriteFrame::Character::KYOJIN_2,
        Resource::SpriteFrame::Character::KYOJIN_3,
        Resource::SpriteFrame::Character::KYOJIN_4,
    };
    
    this->soundFilePaths = {};
    
    return true;
}