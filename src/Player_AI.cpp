#include "Player_AI.h"

Player_AI::Player_AI()
{
    //ctor
}
Player_AI::Player_AI(int number,Uint16 windows_width, Uint16 windows_height,SDL_Event* pevent,Basic_Images* fond,SDL_Surface* screen):
Player(number,windows_width,windows_height,pevent,fond,screen)
{
    //ctor
}

Player_AI::~Player_AI()
{
    //dtor
}

Player_AI::Player_AI(const Player_AI& other)
{
    //copy ctor
}

Player_AI& Player_AI::operator=(const Player_AI& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
Cards* Player_AI::Choose_Card(const std::array<Cards*,4>& trick,int i_master)
{
    return what_card_do_i_play(trick,i_master);
}
int Player_AI::Take(bool first_round,int color_proposed,int height_proposed)
{
    return do_i_take(first_round,color_proposed,height_proposed);
}
