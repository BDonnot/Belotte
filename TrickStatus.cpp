#include "TrickStatus.h"
void TrickStatus::Update(const std::array<Cards*,4>& trick_in_progress
                         ,bool hasCol
                         ,bool hasTrump)
{
    _trumpColor = _infos.TrumpColor();
    Cards* pcard_ref = trick_in_progress[0];
    _colour_asked = pcard_ref->GetColour(); //the color asked
    _has_col = hasCol; //do I have the color asked
    _has_trump = hasTrump;//do I have trump
    _myPosition = _infos.NumberCardsPlayed(); //my position in the trick
    _winner = _infos.StrongestCard(); //the position of the winner (in the trick)
    _maxHeight = trick_in_progress[_winner]->GetHeight(); //height of the winning cards
    _maxColor = trick_in_progress[_winner]->GetColour(); //color of the winning cards
}
