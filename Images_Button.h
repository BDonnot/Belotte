#ifndef IMAGES_BUTTON_H
#define IMAGES_BUTTON_H
#include "Images_Sprite.h"
#include "Images_Click.h"
#include "Images_Text.h"
class Images_Button:public Images_Sprite<2>,public Basic_Click //changes color when mouse is over (or clicked on) and we can click on
{
    protected:
        Images_Text _text_on;
        Images_Text _text_off;
        bool _persistant;
    public:
        Images_Button();
//        Images_Button(SDL_Event* event,std::string text_button,Uint16 pos_x,Uint16 pos_y,POSITION_IMAGE pos_option);
//        Images_Button(SDL_Event* event,Uint16 pos_x,Uint16 pos_y,POSITION_IMAGE pos_option,std::string path,int color);
        Images_Button(SDL_Event* event,std::string text_button,PositionGraphic pos);
        Images_Button(SDL_Event* event,PositionGraphic pos,std::string path,int color);
        virtual ~Images_Button();
        virtual void Update_on_it();
        virtual void Display(SDL_Surface* destination);
        virtual void SetPosition(PositionGraphic& pos);

    protected :
        virtual void act();
        void PositionText();
    private:
};

#endif // IMAGES_BUTTON_H
