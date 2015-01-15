//#include "Images_SpriteEvent.h"

//Images_SpriteEvent::Images_SpriteEvent()
//{
//    //ctor
//}
//Images_SpriteEvent::Images_SpriteEvent(std::string path, int numberOfSprites, SDL_Event* pevent):
//    Images_Sprite(path,numberOfSprites)
//    ,Basic_Click(pevent)
//{
//    //ctor
//}
//
//Images_SpriteEvent::~Images_SpriteEvent()
//{
//    //dtor
//}
//
//void Images_SpriteEvent::Update_on_it()
//{
//    if( _pEvent->type == SDL_MOUSEMOTION )
//    {
//        Uint pos_mouse_x = _pEvent->motion.x;
//        Uint pos_mouse_y = _pEvent->motion.y;
//        const Uint& _pos_x = _pos.Getx();
//        const Uint& _pos_y = _pos.Gety();
//        if (_on_it&&!((pos_mouse_x>_pos_x)&&( pos_mouse_x < _pos_x+_width)&&( pos_mouse_y > _pos_y)&&(pos_mouse_y < _pos_y+_height))) _on_it = false;
//        else
//        {
//            if (!_on_it&&((pos_mouse_x>_pos_x)&&( pos_mouse_x<_pos_x+_width)&&(pos_mouse_y >_pos_y)&&(pos_mouse_y<_pos_y+_height))) _on_it = true;
//        }
//    }
//    Set_click_on_long();
//    Set_click_on_short();
//}
//
//Images_SpriteEvent::Images_SpriteEvent(const Images_SpriteEvent& other)
//{
//    //copy ctor
//}
//
//Images_SpriteEvent& Images_SpriteEvent::operator=(const Images_SpriteEvent& rhs)
//{
//    if (this == &rhs) return *this; // handle self assignment
//    //assignment operator
//    return *this;
//}
