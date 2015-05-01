#include <ctime>
#include <iostream>
#include <fstream>

#include "Definitions.h"
#include "Basic_Game_Info.h"

#include "Game_Coinche.h"

using namespace std;

//TO DO : cleaner "data.h"

int main( int argc, char* args[] )
{
    Basic_Game_Info _info;

    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Surface* _screen = nullptr;
    SDL_EnableUNICODE( SDL_ENABLE );
    _screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE);
    TTF_Init();
    SDL_WM_SetCaption("Belote 1.1", NULL);

#if TEST_PLAY_GAME > 0
    for(Uint i = 26; i < 10000; ++i)
    {
        printf("\n\n\nHERE %d\n",i);
        srand(i); //2,7,13,28
        //srand((time(NULL)));
        for (int i = 0; i < 100; i++) {std::rand();}
        Game_Coinche game(_screen,SCREEN_WIDTH,SCREEN_HEIGHT);
        try
        {
            game.Play();
        }
        catch(...)
        {
            printf("---error for %d ---\n",i);
            continue;
        }
    }
#else
        srand(1); //2,7,13,28
        //srand((time(NULL)));
        for (int i = 0; i < 100; i++) {std::rand();}
        Game_Coinche game(_screen,SCREEN_WIDTH,SCREEN_HEIGHT);
        game.Play();
#endif // TEST_PLAY_GAME

    SDL_EnableUNICODE( SDL_DISABLE );
    SDL_Quit();
    return 0;
}
