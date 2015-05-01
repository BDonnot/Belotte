#include "TrickStatus.h"
void TrickStatus::Update(const TrickBasic_Memory&  trick
                    ,bool hasCol
                    ,bool hasTrump)
{
    _trumpColor = _infos.TrumpColor();
    _has_col = hasCol; //do I have the color asked
    _has_trump = hasTrump;//do I have trump
    _myPosition = Position_Trick(trick.NumberCardsPlayed()); //my position in the trick
}
