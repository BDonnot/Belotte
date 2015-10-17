#include "Display_Trump.h"
using namespace std;
using namespace SDL;

Display_trump::Display_trump()
{
    _trunk = -1;
    _taker = -1;
    _color={255,150,0};
    _font = TTF_OpenFont( "fonts/MLSGY.ttf", 40 );

    _names[0] = "";
    _names[1] = "";
    _names[2] = "";
    _names[3] = "";

    _clip[ 0 ].x = 0;
    _clip[ 0 ].y = 0;
    _clip[ 0 ].w = 100;
    _clip[ 0 ].h = 100;

    _clip[ 1 ].x = 150;
    _clip[ 1 ].y = 0;
    _clip[ 1 ].w = 100;
    _clip[ 1 ].h = 100;

    _clip[ 2 ].x = 0;
    _clip[ 2 ].y = 150;
    _clip[ 2 ].w = 100;
    _clip[ 2 ].h = 100;

    _clip[ 3 ].x = 150;
    _clip[ 3 ].y = 150;
    _clip[ 3 ].w = 100;
    _clip[ 3 ].h = 100;

    _surface = load_sprite("images/sprite_trunk.png");
    _surface_name = NULL;
}

Display_trump::~Display_trump()
{
    SDL_FreeSurface(_surface);
    SDL_FreeSurface(_surface_name);
}

SDL_Surface* Display_trump::load_sprite( std::string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
        if( optimizedImage != NULL )
        {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0,0xff,0xff ) );
        }
    }
    return optimizedImage;
}

void Display_trump::Apply_sprite(SDL_Surface* destination )
{
    if (_trunk != -1)
    {
        SDL_Rect offset;
        offset.x = 0;
        offset.y = 0;
        SDL_BlitSurface( _surface, &_clip[_trunk], destination, &offset );
    }
    if (_taker != -1)
    {
        int w = _surface_name->w;
        int h = _surface_name->h;
        SDL_Rect offset;
        offset.x = 50-w/2;
        offset.y = 50-h/2;
        SDL_BlitSurface( _surface_name, NULL, destination, &offset );
    }
}

void Display_trump::Set_color(int color)
{
    _trunk = color;
}

void Display_trump::Set_names(string name0,string name1,string name2,string name3)
{
    _names[0] = name0;
    _names[1] = name1;
    _names[2] = name2;
    _names[3] = name3;
}

void Display_trump::Set_taker(int taker)
{
    _taker = taker;
    if (taker != -1) _surface_name = TTF_RenderText_Solid( _font, _names[_taker].c_str(), _color);
    else
    {
        if (_surface_name != NULL) SDL_FreeSurface(_surface_name);
    }

}

void Display_trump::Set_event(SDL_Event* pevent)
{
    _pEvent = pevent;
}

void Display_trump::Update()
{
    int pos_mouse_x = -1;
    int pos_mouse_y = -1;
    if( _pEvent->type == SDL_MOUSEMOTION )
    {
        pos_mouse_x = _pEvent->motion.x;
        pos_mouse_y = _pEvent->motion.y;
        if ((_on_it != 0)&&!(( pos_mouse_x < 100)&&(pos_mouse_y < 100))) _on_it = 0;
        else
        {
            if (( pos_mouse_x < 100)&&(pos_mouse_y < 100)) _on_it += 1;
        }
    }
}
