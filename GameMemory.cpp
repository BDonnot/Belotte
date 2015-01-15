#include "GameMemory.h"
/*
GameMemory::~GameMemory()
{
    //dtor
}
*/
GameMemory::GameMemory():
    _infos()
{
    _orderTrump = {JACK,NINE,ACE,TEN,KING,QUEEN,EIGHT,SEVEN};
    _orderNoTrump = {ACE,TEN,KING,QUEEN,JACK,NINE,EIGHT,SEVEN};
    Reset();
}

void GameMemory::Reset()
{
    for(Uint i = 0; i < 4; ++i)
    {
        for(Uint j = 0; j < 8; ++j)
        {
            _fallenCards[i][j] = false;
        }
    }
    for(Uint i = 0; i < 4; ++i)
    {
        for(Uint j = 0; j < 4; ++j)
        {
            _playerStillHaveColor[i][j] = true;
        }
    }
    _heightMaster = {ACE,ACE,ACE,ACE};
    for(Uint i = 0; i < 4; ++i)
    {
        if(_infos.ConvertIntToColor(i)==_infos.TrumpColor()) _heightMaster[i] = JACK;
    }
}

void GameMemory::TellTrick(const std::array<Cards*,4>& trick,POSITION_PLAYER firstToPlay)
{
    POSITION_PLAYER player = firstToPlay;
    for(auto it = trick.begin(); it != trick.end(); ++it)
    {
        if((*it)==NULL) return;
        updateMemory(*it,trick[0]->GetColour(),player);
        player = _infos.Next(player);
    }
}

void GameMemory::updateMemory(Cards* currentCard,CARDS_COLOR colorFirst,POSITION_PLAYER playerPlayed)
{
    CARDS_COLOR color = currentCard->GetColour();
    const Uint iColor = _infos.ColorToInt(color);
    CARDS_HEIGHT height = currentCard->GetHeight();
    Uint iHeight = _infos.HeightToInt(height);
    Uint iPlayer = _infos.PosPlayerToInt(playerPlayed);
    _fallenCards[iColor][iHeight] = true;
    if(color != colorFirst) _playerStillHaveColor[iPlayer][iColor]= false;

    if(_heightMaster[iColor] == height) //if the cards master in a color is played
    {
         Uint iOrder = 0;
         std::array<CARDS_HEIGHT,8>& cardsOrder(_orderNoTrump);
         if(color == _infos.TrumpColor()) cardsOrder = _orderTrump;
         const std::array<bool,8>& fallenCards(_fallenCards[iColor]);

         while(cardsOrder[iOrder] != height){iOrder++;}
         while(fallenCards[iHeight])
         {
            iHeight = _infos.HeightToInt(cardsOrder[iOrder]);
            iOrder++;
         }
         _heightMaster[iColor] = cardsOrder[iOrder];
         printf("modify \n");
    }
    std::string test = "color :";
    test += IntToString(iColor);
    test += "; height :";
    test += IntToString(_infos.HeightToInt(_heightMaster[iColor]));
    test += "\n";
    printf(test.c_str());
}
