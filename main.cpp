#include <ctime>
#include <iostream>
#include <fstream>

#include "Definitions.h"
#include "Basic_Game_Info.h"

using namespace std;
//#include "Jeu.h"

int SCREEN_BPP=32;
int FRAMES_PER_SECOND = 32;
int BASIS = 0;
int BENJAMIN = 1;
int LOIC = 2;
int MORGANE = 4;
int SIMON = 3; //un ami qui aime beaucoup la belote
#include "Images_Move.h"
#include "Images_Sprite.h"
#include "Images_Transparency.h"
#include "Windows_Belot.h"
#include "Images_Text.h"
#include "Text_Typing.h"
#include "Player_Name.h"
#include "Images_Button.h"
#include "Cards.h"
#include "Player.h"
#include "Player_Human.h"
#include "Cards_Deck.h"
#include "Game_Coinche.h"
#include "Bidding.h"
#include "Save_Game.h"

//TO DO : cf mail de simon du 15 mars
//TO DO : cf mail de papa du 17 mars

//TO DO : minimiser les classes UI dependent

//TO DO : ouvrir est souvent une bonne idée
//TO DO : jouer atout quand son partenaire a pris !!!
//TO DO : mettre ses AS quand on a la main et qu'on est en defense
//TO DO : ne pas mettre des points quand le plis n'est pas remporte

//eviter de jouer une couleur dans laquelle on a un 10
//TO DO : ne pas lancer son dernier atout
//TO DO : ne pas défausser ses AS : essayer de garder ses cartes maitresses !
//TO DO : cleaner "data.h"
int main( int argc, char* args[] )
{
//    ofstream output("datas/games.blt", ios::app);
    Basic_Game_Info _info;
    srand((time(NULL)));
    //srand(7);
    for (int i = 0; i < 100; i++) {std::rand();}
    SDL_Surface* _screen = nullptr;
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_EnableUNICODE( SDL_ENABLE );
//    SDL_Event* _pEvent = new SDL_Event;
    _screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_RESIZABLE);
//    Uint8* _keystates = SDL_GetKeyState( nullptr );
    TTF_Init();
    SDL_WM_SetCaption("Belote 1.1", NULL);

    Game_Coinche game(_screen,SCREEN_WIDTH,SCREEN_HEIGHT);
    game.Play();

    //Images_Text _text_on("fonts/MLSGY.ttf",24,(SDL_Color) {255,255,255}, "tuut");

//    Save_Game game;
//    game.EndGame();
//    output << "toto";
//    delete _keystates;

    SDL_EnableUNICODE( SDL_DISABLE );
    SDL_Quit();
//    output.close();
    return 0;
}
//test_jeu(1000);
//test_fait_entree(9834);
//1 000 000 de parties en 2300 secondes
