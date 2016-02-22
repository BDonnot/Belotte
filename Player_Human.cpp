#include "Player_Human.h"
using namespace::std;
#if COMPLETE_GAME > 0
//Event/Display
void Player_Human::Update_Mouse(GAME_PHASES currentPhase)
{
    _name.Update(currentPhase);
    if (currentPhase == PLAYING)
    {
        _UpdateCardMouse.Reset();
        for (auto it = _playable_cards.rbegin(); it !=_playable_cards.rend(); ++it) //run through the list backward, because the last card displayed is in front of the other
        {
            _UpdateCardMouse(*it);
        }
    }
    if (currentPhase == BIDDING)
     {
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
Cards* Player_Human::what_card_do_i_play(const TrickBasic_Memory& trick)
{
   Cards *pcard = nullptr;
    if (_UpdateCardMouse.Click() != 255)
    {
        std::list<Cards*>::reverse_iterator it = _playable_cards.rbegin();
        for (Uint8 i = 0; i < _UpdateCardMouse.Click() ; i++)
        {
            ++it;
        }
        pcard = *it;
        _UpdateCardMouse.Reset();
    }
    return pcard;
}

//Take
void Player_Human::updateBid(const BetsMemory& bets)
{
    //TO DO : ask if Coinche
    const Player_Bid& bid = _TakeCoinche.Has_Taken();
    _currentBid.Bid(bid.Color(),bid.Bid());
}

void Player_Human::ResetBid(bool betFinished)
{
    _TakeCoinche.Reset(betFinished);
    _currentBid.Reset();
    if(!betFinished) return;
    _oldBid.Reset();
}

#endif //#if COMPLETE_GAME > 0
