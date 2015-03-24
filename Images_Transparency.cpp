#include "Images_Transparency.h"
using namespace::std;
Images_Transparency::Images_Transparency()
{
    //ctor
}
Images_Transparency::Images_Transparency(string path):Basic_Images(path),Basic_Transparency()
{
}
Images_Transparency::Images_Transparency(string path, Uint16 transparency):Basic_Images(path),Basic_Transparency(transparency)
{
}

Images_Transparency::~Images_Transparency()
{
    //dtor
}
void Images_Transparency::act()
{
    transparency();
    SDL_SetAlpha( _surface.get(), SDL_SRCALPHA | SDL_RLEACCEL, _alpha );
}
