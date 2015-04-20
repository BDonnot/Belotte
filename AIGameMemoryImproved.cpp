#include "AIGameMemoryImproved.h"

void AIGameMemoryImproved::updateSmarter(const TrickBasic_Memory& trick, const Position_Trick& posTrick)
{
    Card_Color colorAsked = trick.ColorAsked();
    Card_Color trumpColor = _infos.TrumpColor();
    Player_ID currentPlayer = _infos.FirstToPlay(posTrick,_posPlayer); //TO DO : move trick basic memory
    Card_Color tempCol;
    Card_Height tempHeig;
    Cards_Basic tempCard;
    for(Uint i = 0; i < 4; ++i)
    {
        tempCol = trick[i]->GetColour();
        tempHeig = trick[i]->GetHeight();
        _canPlayersHaveCard.SetFallen(*trick[i]);
        if(tempCol == trumpColor)
        {
            dealWithTrumps(i,trick,currentPlayer,trumpColor);
        }
        if(tempCol != colorAsked)
        {
            _canPlayersHaveCard.SetCutColor(currentPlayer,colorAsked);
        }
        currentPlayer.Next();
    }

}

void AIGameMemoryImproved::dealWithTrumps(Uint i,const TrickBasic_Memory& trick,const Player_ID& currentPlayer,const Card_Color& trumpColor )
{
    Card_Color tempCol;
    Card_Height tempHei;
    for(Uint j = 0; j < i; ++j)
    {
        tempCol = trick[j]->GetColour();
        tempHei = trick[j]->GetHeight();
        if(tempCol == trumpColor && trick[i]->Win(tempHei))
        {
            /**
            **If the player have not go up at trump
            **it is because he has not higher !
            **/
            for(Uint icolTrump = 0; icolTrump < 8; ++icolTrump)
            {
                Card_Height tempHei2(icolTrump);
                if(!trick[i]->Win(tempHei2))
                {
                    _canPlayersHaveCard.SetCanHaveCard(currentPlayer,trumpColor,tempHei2);
                }

            }
        }
    }
}
