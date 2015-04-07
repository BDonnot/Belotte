#include "Player_AI.h"
/*
Player_AI::Player_AI(PLAYER_ID number,Uint16 windows_width, Uint16 windows_height,SDL_Event* pevent,Basic_Images* fond,SDL_Surface* screen):
Player(number,windows_width,windows_height,pevent,fond,screen)
,_rand(2520)
{
    //ctor
}
*/
/*
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
*/
/*
Cards* Player_AI::Choose_Card(const std::array<Cards*,4>& trick,int i_master)
{
    for (auto it = _hand.begin(); it != _hand.end(); ++it)
    {
        //update _playble_cards
    }
    return what_card_do_i_play(trick,i_master);
}
int Player_AI::Take(bool first_round,int color_proposed,int height_proposed)
{
    return do_i_take(first_round,color_proposed,height_proposed);
}
*/


/*
CARDS_COLOR Player_AI::do_i_take(bool first_round,CARDS_COLOR color_proposed,Card_Height height_proposed) //by default, take if 3 (or more) cards in the color proposed
{
    int number_cards = 0;
    if (first_round)
    {
        number_cards = how_many_colour(color_proposed);
        return ((number_cards>3) ? color_proposed: NO);
    }
    for(int i = 0; i < 4; i++)
    {
        CARDS_COLOR res = do_i_take(true,_basic_info.ConvertIntToColor(i),height_proposed);
        if((res!=NO)&&(res!=color_proposed))
            return res;
    }
    return NO;
}

std::list<Cards*>::iterator Player_AI::what_card_do_i_play(const std::array<Cards*,4>& trick) //by default, play a random card
{
//    return _hand.front();
    Uint n = _playable_cards.size();
    switch(n)
    {
    case 0 :
        return _hand.end();
    case 1:
        return _playable_cards.front();
    default :
        auto it = _playable_cards.begin();
        std::advance(it,_rand.generate_number()%(n-1));
        return (*it);
    }
    //Random rand(_playable_cards.size()-1);
}

void Player_AI::UpdateBid()
{
//    if(_rand.generate_number() > 5)
//    {
//        _currentBid.Bid(_basic_info.ConvertIntToColor(_rand.generate_number()%4),_basic_info.MaxBid()+10);
//    }
//    else _currentBid.Bid(NO,_basic_info.MaxBid());
    if (_number == 1) _currentBid.Bid(_basic_info.ConvertIntToColor(_rand.generate_number()%4),_basic_info.MaxBid()+10);
    else _currentBid.Bid(NO,_basic_info.MaxBid()+10);
//    _currentBid.Bid(NO,_basic_info.MaxBid()+10);
}
*/
