#include "AIGameMemory.h"

AIGameMemory::~AIGameMemory()
{
    //dtor
}
void AIGameMemory::UpdateFullTrick(const TrickBasic_Memory& trick, const Position_Trick& posTrick)
{
    Uint i;
    Card_Color colorAsked = trick.ColorAsked();
    Uint intCol = colorAsked.ToInt();
    _nbColorPlayed[intCol] += 1;
    Card_Color currentColor;
    Card_Height currentHeight;
    PLAYER_ID currentPlayer = _infos.FirstToPlay(posTrick,_posPlayer);
    bool modify = false;
    for(i = 0; i < 4; i++)
    {
        currentColor = trick[i]->GetColour();
        currentHeight = trick[i]->GetHeight();
        intCol = currentColor.ToInt();
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
        if(currentColor != colorAsked)
        {
            _playerCut.SetCut(currentPlayer,currentColor );
            if(trick[i]->Value() <= 4) _playerCalls.SetCut(currentPlayer,currentColor ); //we don't call with TEN, NINE or ACE ...
            //maybe a smarter option here
        }
    }
    //update everything else
    updateEverythingElse(_infos.FirstToPlay(posTrick,_posPlayer));

}
void AIGameMemory::InitEverything()
{
    _fallenCards.Reset();
    _playerCalls.Reset();
    _playerCut.Reset();
    for(int i = 0; i < 4; ++i)
    {
        _nbColorPlayed[i] = 0;
        Card_Color col(i);
        _heightsMaster[i] = (col == _infos.TrumpColor() ? JACK : ACE);
    }
    updateEverythingElse(_posPlayer);
}
void AIGameMemory::computeNewHeightMaster()
{
    for(Uint i = 0; i < 4; i++)
    {
        Card_Color currentColor(i);
        bool isTrump = (currentColor == _infos.TrumpColor() );
        Card_Height currentMaster = _heightsMaster[i];
        Card_Height seven(SEVEN);
        while(_fallenCards.IsFallen(currentColor,currentMaster) && currentMaster != seven)
        {
            currentMaster = currentMaster.HeightUnder(isTrump);
        }
        _heightsMaster[i] = currentMaster;
    }
}
void AIGameMemory::updateEverythingElse(PLAYER_ID firstToPlay)
{
    //INITIALIZE THE ARRAYS
    for(Uint i = 0; i < 4; i++)
    {
        _nbRemaining[i] = 0;
        _greatest[i] = SEVEN;
        _smallest[i] = ACE;

        _IAmMaster[i] = false;
        _HaveTen[i] = false;

        _protectPoints[i] = false;
        _longe[i] = 0;
    }

    std::list<const Cards*> _cardsPerColor[4];
    Card_Color color(_infos.TrumpColor());
    Card_Height height(UNINTIALIZED);
    Uint iColor = color.ToInt();
    _greatest[iColor] = JACK;
    //COMPUTE THE THINGS DIRECTLY FROM THE HAND
    for(const Cards* pcard : *_pHand)
    {
        color = pcard->GetColour();
        height = pcard->GetHeight();
        iColor = color.ToInt();
        _nbRemaining[iColor] += 1;
        if(pcard->Win(_greatest[iColor])) _greatest[iColor] = height;
        if(!(pcard->Win(_smallest[iColor]))) _smallest[iColor] = height;
        if(height == (color == _infos.TrumpColor() ? Card_Height(NINE) : Card_Height(TEN))) _HaveTen[iColor] = true;
        _cardsPerColor[iColor].push_back(pcard);
    }

    //COMPUTE THE OTHER THINGS COLOR PER COLOR
    for(Uint i = 0; i < 4; i++)
    {
        color = Card_Color(i);
        Uint nbLeft = 8-_fallenCards.ComputeFallen(color);
        if(_nbRemaining[i] == nbLeft)
        {
            PLAYER_ID currentPlayer = firstToPlay;
            for(i= 1; i < 4; i++)
            {
                currentPlayer = _infos.Next(currentPlayer);
                _playerCut.SetCut(currentPlayer,color );
            }
        }
        if (_greatest[i] == _heightsMaster[i] )_IAmMaster[i] = true;
        computeScoreLongeAndProtectPoint(_cardsPerColor[i],_longe[i],_protectPoints[i]);
    }

}
void AIGameMemory::computeScoreLongeAndProtectPoint(std::list<const Cards*>& cardsInTheColor,Uint& scoreLonge, bool& protectPoints)
{
    protectPoints = false;
    scoreLonge = 0;
}
bool AIGameMemory::callCut(PLAYER_ID player,const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const
{
    return StoreCallCut.Cut(player,color);
}
bool AIGameMemory::opponentsCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const
{
    PLAYER_ID opp1 =  _infos.Next(_posPlayer);
    PLAYER_ID opp2 =  _infos.Next(_infos.Next(opp1));
    return StoreCallCut.Cut(opp1,color) || StoreCallCut.Cut(opp2,color) ;
}
bool AIGameMemory::teammateCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const
{
    PLAYER_ID teammate =  _infos.Next(_infos.Next(_posPlayer));
    return StoreCallCut.Cut(teammate,color);
}
bool AIGameMemory::nextCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const
{
    PLAYER_ID nextPlayer =  _infos.Next(_posPlayer);
    return StoreCallCut.Cut(nextPlayer,color) ;
}
bool AIGameMemory::Cut(PLAYER_ID player,const Card_Color& color) const //Do 'player' cut at 'color'
{
    return callCut(player,color,_playerCut);
}
bool AIGameMemory::OpponentsCut(const Card_Color& color) const
{
    return opponentsCallCut(color,_playerCut);
}
bool AIGameMemory::TeammateCut(const Card_Color& color) const
{
    return teammateCallCut(color,_playerCut);
}
bool AIGameMemory::NextCut(const Card_Color& color) const
{
    return nextCallCut(color,_playerCut);
}
bool AIGameMemory::Call(PLAYER_ID player,const Card_Color& color) const //Do 'player' call at 'color'
{
    return callCut(player,color,_playerCalls);
}
bool AIGameMemory::OpponentsCall(const Card_Color& color) const
{
    return opponentsCallCut(color,_playerCalls);
}
bool AIGameMemory::TeammateCall(const Card_Color& color) const
{
    return teammateCallCut(color,_playerCalls);
}
bool AIGameMemory::NextCall(const Card_Color& color) const
{
    return nextCallCut(color,_playerCalls);
}
Card_Height AIGameMemory::Master(const Card_Color& color) const
{
    return(_heightsMaster[color.ToInt()]);
}
Uint AIGameMemory::NbFallen(const Card_Color& color) const
{
    Uint nbFallen = _fallenCards.ComputeFallen(color);
    return static_cast<Uint>(8-nbFallen);
}
Uint AIGameMemory::ScoreLonge(const Card_Color& color) const
{
    Uint iColor = color.ToInt();
    return _longe[iColor];
}
bool AIGameMemory::AllRemainingCards(const Card_Color& color) const
{
    Uint iColor = color.ToInt();
    Uint nbFallen = _fallenCards.ComputeFallen(color);
    return nbFallen + _nbRemaining[iColor] == 8;
}
bool AIGameMemory::AmIMaster(const Card_Color& color) const //Have I the strongest card in the color
{
    Uint iColor = color.ToInt();
    return _IAmMaster[iColor];
}
bool AIGameMemory::DoMyTeamTook() const
{
    PLAYER_ID teammate = _infos.IntToPosPlayer( _infos.PosPlayerToInt(_posPlayer)+2 %4 );
    return (_posPlayer == _infos.Taker()) || (teammate == _infos.Taker());
}
bool AIGameMemory::CardsFallen(const Card_Color& color,const Card_Height& height) const
{
    return _fallenCards.IsFallen(color,height);
}

bool AIGameMemory::HaveTen(const Card_Color& color) const
{
    Uint iColor = color.ToInt();
    return _HaveTen[iColor];
}
bool AIGameMemory::ProtectPoints(const Card_Color& color) const
{
    Uint iColor = color.ToInt();
    return _protectPoints[iColor];
}
Uint AIGameMemory::NbColorPlayed(const Card_Color& color) const //number of time each color have been played (first)
{
    Uint iColor = color.ToInt();
    return _nbColorPlayed[iColor];
}
Card_Height AIGameMemory::Greatest(const Card_Color& color) const //return the greatest cards I have in the color
{
    Uint iColor = color.ToInt();
    return _greatest[iColor];
}
Card_Height AIGameMemory::Smallest(const Card_Color& color) const //return the smallest cards I have in the color
{
    Uint iColor = color.ToInt();
    return _smallest[iColor];
}
