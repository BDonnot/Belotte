#ifndef PLAYER_AI_H
#define PLAYER_AI_H

#include "Player.h"


class Player_AI : public Player //just to do the interface between a player and all the type of AI possible
{
    public:
        Player_AI();
        Player_AI(int number,Uint16 windows_width, Uint16 widows_height,SDL_Event* pevent,Basic_Images* fond,SDL_Surface* screen);
        virtual ~Player_AI();
        Cards* Choose_Card(const std::array<Cards*,4>& trick,int i_master); //choose what card the player play
        int Take(bool first_round,int color_proposed,int height_proposed); //choose if the player take or not. 127 : no, 255 : not choosen, 0-3 : color at which the player wants to take
    protected:
        int do_i_take(bool first_round,int color_proposed,int height_proposed);
        Cards* what_card_do_i_play(const std::array<Cards*,4>& trick,int i_master);

    private:
        Player_AI(const Player_AI& other);
        Player_AI& operator=(const Player_AI& other);
};

#endif // PLAYER_AI_H
