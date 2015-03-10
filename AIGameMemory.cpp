#include "AIGameMemory.h"
/*
AIGameMemory::AIGameMemory()
{
    //ctor
}
*/
AIGameMemory::~AIGameMemory()
{
    //dtor
}

void AIGameMemory::UpdateFullTrick(const std::array<Cards*,4>& trick, POSITION_TRICK posTrick)
{
    Uint i;
    CARDS_COLOR colorAsked = trick[0]->GetColour();
    CARDS_COLOR currentColor;
    CARDS_HEIGHT currentHeight;
    POSITION_PLAYER currentPlayer = _infos.FirstToPlay(posTrick,_posPlayer);
    bool modify = false;
    for(i = 0; i < 4; i++)
    {
        currentColor = trick[i]->GetColour();
        currentHeight = trick[i]->GetHeight();
        Uint intCol = _infos.ColorToInt(currentColor);
        //update the fallen Cards
        _fallenCards.SetFallen(currentColor,currentHeight);
        //in this case we need to update the height master
        if(currentHeight == _heightsMaster[intCol]) modify = true;
    }
    //update the masters cards if need to be done
    if(modify) computeNewHeightMaster();

    //update who cuts where
    for(i= 1; i < 4; i++)
    {
        currentPlayer = _infos.Next(currentPlayer);
        currentColor = trick[i]->GetColour();
        if(currentColor != colorAsked) _playerCut.SetCut(currentPlayer,currentColor );
    }
    //update the playerCut
    updatePlayerCutSmarter(_infos.FirstToPlay(posTrick,_posPlayer));

}

void AIGameMemory::InitEverything()
{
    for(int i = 0; i < 4; ++i)
    {
        CARDS_COLOR col = _infos.IntToColor(i);
        _heightsMaster[i] = (col == _infos.TrumpColor() ? JACK : ACE);
    }
}
CARDS_HEIGHT AIGameMemory::heightUnder(CARDS_HEIGHT height,bool isTrump)
{
    switch(height)
    {
        case SEVEN :
            return SEVEN;
        case EIGHT :
            return SEVEN;
        case NINE :
            return isTrump ? ACE : EIGHT ;
        case TEN :
            return KING;
         case JACK :
            return NINE;
        case QUEEN :
            return isTrump ? EIGHT : JACK;
        case KING :
            return QUEEN;
        case ACE :
            return TEN;
        default :
            return UNINTIALIZED;
    }
}

void AIGameMemory::computeNewHeightMaster()
{
    for(Uint i = 0; i < 4; i++)
    {
        CARDS_COLOR currentColor = _infos.IntToColor(i);
        bool isTrump = (currentColor == _infos.ColorToInt(_infos.TrumpColor()) );
        CARDS_HEIGHT currentMaster = _heightsMaster[i];

        while(_fallenCards.IsFallen(currentColor,currentMaster) && currentMaster!= SEVEN)
        {
            currentMaster = heightUnder(currentMaster,isTrump);
        }
        _heightsMaster[i] = currentMaster;
        //char forPrinting[100];
        //sprintf(forPrinting,"new height master for color %d is %d\n",i,_infos.HeightToInt(currentMaster));
        //printf(forPrinting);
    }
}

void AIGameMemory::updatePlayerCutSmarter(POSITION_PLAYER firstToPlay)
{
    for(Uint i = 0; i < 4; i++)
    {
        CARDS_COLOR currentColor = _infos.IntToColor(i);
        Uint nbLeft = 8-_fallenCards.ComputeFallen(currentColor);
        Uint nbHand = 0;
        for(auto pcards : *_pHand)
        {
            if(pcards->GetColour() == currentColor) ++nbHand;
        }
        if(nbHand == nbLeft)
        {
            POSITION_PLAYER currentPlayer = firstToPlay;
            for(i= 1; i < 4; i++)
            {
                currentPlayer = _infos.Next(currentPlayer);
                _playerCut.SetCut(currentPlayer,currentColor );
            }
            //char forPrinting[100];
            //sprintf(forPrinting,"everybody cut at %d\n",i);
            //printf(forPrinting);
        }
    }
}
