#include "Images_Transparency.h"
using namespace::std;

void Images_Transparency::act()
{
    transparency();
    SDL_SetAlpha( _surface.get(), SDL_SRCALPHA | SDL_RLEACCEL, _alpha );
}
