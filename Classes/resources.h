//
//  resources.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/04.
//
//

#ifndef resources_h
#define resources_h

namespace Resource
{
    namespace Font
    {
        static const std::string BASE_PATH = "fonts/";
        static const std::string SYSTEM = BASE_PATH + "aoyagireisyosimo_ttf_2_01.ttf";
        static const std::string MESSAGE = BASE_PATH + "cinecaption2.28.ttf";
    };
    
    namespace SE
    {
        static const std::string BASE_PATH = "se/";
        static const std::string BACK = "back.mp3";
        static const std::string CURSOR_MOVE = "cursorMove.mp3";
        static const std::string GET_ITEM = "mystery5.mp3";
        static const std::string TITLE_ENTER = "title-enter.mp3";
        static const std::string TROPHY_NOTIFICATION = "receipt05.mp3";
        static const std::string LOAD = "load.mp3";
        static const std::string GAME_START = "gameStart.mp3";
        static const std::string LOGO = "logo_display.mp3";
        static const std::string REACTION = "reaction.mp3";
    };
    
    namespace BGM
    {
        static const std::string BASE_PATH = "BGM/";
        static const std::string TIRED = "tired.mp3";
    };
    
    namespace SpriteFrame
    {
        static const std::string BASE_PATH = "img/";
        static const std::string ANIMATION = BASE_PATH + "animation";
        static const std::string BLOOD = BASE_PATH + "blood";
        static const std::string EFFECT = BASE_PATH + "effect";
        static const std::string FRAME = BASE_PATH + "frame";
        static const std::string ICON = BASE_PATH + "icon";
        static const std::string MAP_OBJECT = BASE_PATH + "map_object";
        static const std::string MENU_PANELS = BASE_PATH + "menu_panels";
        static const std::string STARTUP = BASE_PATH + "startup";
        static const std::string TITLE = BASE_PATH + "title";
        static const std::string UI = BASE_PATH + "ui";
        
        namespace Character
        {
            static const std::string BASE_PATH = SpriteFrame::BASE_PATH + "character/";
            static const std::string MAGOICHI = BASE_PATH + "magoichi";
            static const std::string DAIGORO = BASE_PATH + "daigoro";
            static const std::string DANDAN = BASE_PATH + "dandan";
            static const std::string NADESHIKO = BASE_PATH + "nadeshiko";
            static const std::string MANAKA = BASE_PATH + "manaka";
            static const std::string RANMARU = BASE_PATH + "ranmaru";
            static const std::string SHITSUJI = BASE_PATH + "shituji";
            static const std::string YUKI = BASE_PATH + "yuki";
            static const std::string FUJIN = BASE_PATH + "fujin";
            static const std::string KNIFE = BASE_PATH + "knife";
            static const std::string TAIHO = BASE_PATH + "taihou";
            static const std::string T_FUJIN = BASE_PATH + "Tfujin";
            static const std::string W_MANAKA = BASE_PATH + "wManaka";
            static const std::string W_DANDAN = BASE_PATH + "wDandan";
            static const std::string O_RANMARU = BASE_PATH + "oRanmaru";
            static const std::string S_RANMARU = BASE_PATH + "sRanmaru";
            static const std::string D_DAIGORO = BASE_PATH + "dDaigoro";
            static const std::string KYOJIN_1 = BASE_PATH + "kyoujin1";
            static const std::string KYOJIN_2 = BASE_PATH + "kyoujin2";
            static const std::string KYOJIN_3 = BASE_PATH + "kyoujin3";
            static const std::string KYOJIN_4 = BASE_PATH + "kyoujin4";
        };
    };
};

#endif /* resources_h */
