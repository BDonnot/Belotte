#include "Player_Human.h"
using namespace::std;

/*
Player_Human::Player_Human()
{
    //ctor
}
Player_Human::Player_Human(PLAYER_ID number,Uint16 windows_width, Uint16 windows_height,SDL_Event* pevent,Basic_Images* fond,SDL_Surface* screen):
Player(number,windows_width,windows_height,pevent,fond,screen)
//,_Take(pevent,screen,windows_width,windows_height)
,_TakeCoinche(pevent,screen,windows_width,windows_height)
,_rand(rand())
{
    //ctor
}

Player_Human::~Player_Human()
{
    //dtor
}
Player_Human::Player_Human(const Player_Human& other)
{
    //copy ctor
}
Player_Human& Player_Human::operator=(const Player_Human& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
*/
//Event/Display
void Player_Human::Update_Mouse(GAME_PHASES currentPhase)
{
    _name.Update(currentPhase);
    if (currentPhase == PLAYING)
    {
        _UpdateCardMouse.Reset();
        for (auto it = _playable_cards.rbegin(); it !=_playable_cards.rend(); ++it) //run through the list backward, because the last card displayed is in front of the other
        {
            _UpdateCardMouse(**it);
        }
    }
    if (currentPhase == BIDDING)
     {
         //_Take.Update();
        _TakeCoinche.UpdateMouse(currentPhase);
        _oldBid.UpdateEvent();
     }
     if(currentPhase == AFTER_BET) _oldBid.UpdateEvent();
}
void Player_Human::Display(GAME_PHASES currentPhase)
{
    _oldBid.Display(_screen,currentPhase);
    _name.Display(_screen,currentPhase==SELECT_NAMES);
    _DisplayCardPlayer.Reset(_hand.size());
    for_each(_hand.begin(),_hand.end(),_DisplayCardPlayer);
    _TakeCoinche.Display(currentPhase);
}

//Play
std::list<Cards*>::iterator Player_Human::what_card_do_i_play(const std::array<Cards*,4>& trick)
{
   //_playable_cards = _hand;
   std::list<Cards*>::iterator pcard = _hand.end();
    if (_UpdateCardMouse.Click() != 255)
    {
        auto it = _playable_cards.rbegin();
        for (Uint8 i = 0; i < _UpdateCardMouse.Click() ; i++)
        {
            ++it;
        }
        pcard = *it;
//        for (auto itd = _hand.begin(); itd != _hand.end(); ++itd)
//        {
//            if (*itd == *it) _hand.erase(itd);
//        }
        _UpdateCardMouse.Reset();
    }
    return pcard;
}

//Take
int Player_Human::do_i_take(bool first_round,int color_proposed,int height_proposed) //choose if the player take or not
{
    //classical belote
    return _Take.Has_Taken();
}
void Player_Human::updateBid(const BetsMemory& bets)
{
    //coinche
    const Player_Bid& bid = _TakeCoinche.Has_Taken();
    _currentBid.Bid(bid.Color(),bid.Bid());
}
void Player_Human::ResetBid(bool betFinished)
{
    _TakeCoinche.Reset();
    _currentBid.Reset();
    if(!betFinished) return;
    _lastBidTurn = 0;
    _oldBid.Reset();
    //resetTake();
}
