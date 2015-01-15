#ifndef IMAGES_SPRITEEVENT_H
#define IMAGES_SPRITEEVENT_H

#include "Images_Sprite.h"
#include "Basic_Click.h"

template<Uint numberOfSprite>
class Images_SpriteEvent : public Images_Sprite<numberOfSprite> , public Basic_Click
{
    public:
        Images_SpriteEvent();
        Images_SpriteEvent(std::string path, SDL_Event* pevent);
        virtual ~Images_SpriteEvent(){}
        virtual void Update_on_it();
    protected:
    private:
};

template<Uint numberOfSprite>
Images_SpriteEvent<numberOfSprite>::Images_SpriteEvent():
Images_Sprite<numberOfSprite>()
{
    //ctor
}

template<Uint numberOfSprite>
Images_SpriteEvent<numberOfSprite>::Images_SpriteEvent(std::string path, SDL_Event* pevent):
Images_Sprite<numberOfSprite>(path)
,Basic_Click(pevent)
{
    //ctor
}

template<Uint numberOfSprite>
void Images_SpriteEvent<numberOfSprite>::Update_on_it()
{
    if( _pEvent->type == SDL_MOUSEMOTION )
    {
        Uint pos_mouse_x = _pEvent->motion.x;
        Uint pos_mouse_y = _pEvent->motion.y;
        const Uint& pos_x = (this)->_pos.Getx();
        const Uint& pos_y = (this)->_pos.Gety();
        const Uint& width = (this)->_width;
        const Uint& height = (this)->_height;
        if (_on_it&&!((pos_mouse_x>pos_x)&&( pos_mouse_x < pos_x+width)&&( pos_mouse_y > pos_y)&&(pos_mouse_y < pos_y+height))) _on_it = false;
        else
        {
            if (!_on_it&&((pos_mouse_x>pos_x)&&( pos_mouse_x<pos_x+width)&&(pos_mouse_y >pos_y)&&(pos_mouse_y<pos_y+height))) _on_it = true;
        }
    }
    Set_click_on_long();
    Set_click_on_short();
}

#endif // IMAGES_SPRITEEVENT_H
