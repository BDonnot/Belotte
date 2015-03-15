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
    Uint intCol = _infos.ColorToInt(colorAsked);
    _nbColorPlayed[intCol] += 1;
    CARDS_COLOR currentColor;
    CARDS_HEIGHT currentHeight;
    PLAYER_ID currentPlayer = _infos.FirstToPlay(posTrick,_posPlayer);
    bool modify = false;
    for(i = 0; i < 4; i++)
    {
        currentColor = trick[i]->GetColour();
        currentHeight = trick[i]->GetHeight();
        intCol = _infos.ColorToInt(currentColor);
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
        CARDS_COLOR col = _infos.IntToColor(i);
        _heightsMaster[i] = (col == _infos.TrumpColor() ? JACK : ACE);
    }
    updateEverythingElse(_posPlayer);
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


//        CARDS_HEIGHT _greatest[4];  //key : color -> stock the height of the greatest cards I have per color
//        CARDS_HEIGHT _smallest[4];  //key : color -> stock the height of the smallest cards I have per color
//        Uint _nbRemaining[4]; //key : color ; number of cards remaining in my hand per color
//        Uint _longe[4]; //key : color ; score for the longe (number of cards in the color)
//        bool _IAmMaster[4]; //key : color ; am i the master in the color
//        bool _HaveTen[4]; //key : color ; have I the TEN/NINE (at trump) in the color
//        bool _protectPoints[4]; //key : color; do I have to protect some important point (TEN, NINE for example) at the color

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
    CARDS_COLOR color = _infos.TrumpColor();
    CARDS_HEIGHT height = UNINTIALIZED;
    Uint iColor = _infos.ColorToInt(color);
    _greatest[iColor] = JACK;
    //COMPUTE THE THINGS DIRECTLY FROM THE HAND
    for(const Cards* pcard : *_pHand)
    {
        color = pcard->GetColour();
        height = pcard->GetHeight();
        iColor = _infos.ColorToInt(color);
        _nbRemaining[iColor] += 1;
        if(pcard->Win(_greatest[iColor])) _greatest[iColor] = height;
        if(!(pcard->Win(_smallest[iColor]))) _smallest[iColor] = height;
        if(height == (color == _infos.TrumpColor() ? NINE : TEN)) _HaveTen[iColor] = true;
        _cardsPerColor[iColor].push_back(pcard);
    }

    //COMPUTE THE OTHER THINGS COLOR PER COLOR
    for(Uint i = 0; i < 4; i++)
    {
        color = _infos.IntToColor(i);
        Uint nbLeft = 8-_fallenCards.ComputeFallen(color);
        if(_nbRemaining[i] == nbLeft)
        {
            PLAYER_ID currentPlayer = firstToPlay;
            for(i= 1; i < 4; i++)
            {
                currentPlayer = _infos.Next(currentPlayer);
                _playerCut.SetCut(currentPlayer,color );
            }
            //char forPrinting[100];
            //sprintf(forPrinting,"everybody cut at %d\n",i);
            //printf(forPrinting);
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
bool AIGameMemory::callCut(PLAYER_ID player,CARDS_COLOR color,const MemorizeCutsCalls& StoreCallCut) const
{
    return StoreCallCut.Cut(player,color);
}

bool AIGameMemory::opponentsCallCut(CARDS_COLOR color,const MemorizeCutsCalls& StoreCallCut) const
{
    PLAYER_ID opp1 =  _infos.Next(_posPlayer);
    PLAYER_ID opp2 =  _infos.Next(_infos.Next(opp1));
    return StoreCallCut.Cut(opp1,color) || StoreCallCut.Cut(opp2,color) ;
}

bool AIGameMemory::teammateCallCut(CARDS_COLOR color,const MemorizeCutsCalls& StoreCallCut) const
{
    PLAYER_ID teammate =  _infos.Next(_infos.Next(_posPlayer));
    return StoreCallCut.Cut(teammate,color);
}

bool AIGameMemory::nextCallCut(CARDS_COLOR color,const MemorizeCutsCalls& StoreCallCut) const
{
    PLAYER_ID nextPlayer =  _infos.Next(_posPlayer);
    return StoreCallCut.Cut(nextPlayer,color) ;
}
bool AIGameMemory::Cut(PLAYER_ID player,CARDS_COLOR color) const //Do 'player' cut at 'color'
{
    return callCut(player,color,_playerCut);
}
bool AIGameMemory::OpponentsCut(CARDS_COLOR color) const
{
    return opponentsCallCut(color,_playerCut);
}
bool AIGameMemory::TeammateCut(CARDS_COLOR color) const
{
    return teammateCallCut(color,_playerCut);
}
bool AIGameMemory::NextCut(CARDS_COLOR color) const
{
    return nextCallCut(color,_playerCut);
}
bool AIGameMemory::Call(PLAYER_ID player,CARDS_COLOR color) const //Do 'player' call at 'color'
{
    return callCut(player,color,_playerCalls);
}
bool AIGameMemory::OpponentsCall(CARDS_COLOR color) const
{
    return opponentsCallCut(color,_playerCalls);
}
bool AIGameMemory::TeammateCall(CARDS_COLOR color) const
{
    return teammateCallCut(color,_playerCalls);
}
bool AIGameMemory::NextCall(CARDS_COLOR color) const
{
    return nextCallCut(color,_playerCalls);
}

CARDS_HEIGHT AIGameMemory::Master(CARDS_COLOR color) const
{
    return(_heightsMaster[_infos.ColorToInt(color)]);
}

Uint AIGameMemory::NbFallen(CARDS_COLOR color) const
{
    Uint nbFallen = _fallenCards.ComputeFallen(color);
    return static_cast<Uint>(8-nbFallen);
}

Uint AIGameMemory::ScoreLonge(CARDS_COLOR color) const
{
    Uint iColor = _infos.ColorToInt(color);
    return _longe[iColor];
}

bool AIGameMemory::AllRemainingCards(CARDS_COLOR color) const
{
    Uint iColor = _infos.ColorToInt(color);
    Uint nbFallen = _fallenCards.ComputeFallen(color);
    return nbFallen + _nbRemaining[iColor] == 8;
}

bool AIGameMemory::AmIMaster(CARDS_COLOR color) const //Have I the strongest card in the color
{
    Uint iColor = _infos.ColorToInt(color);
    return _IAmMaster[iColor];
}

bool AIGameMemory::DoMyTeamTook() const
{
    PLAYER_ID teammate = _infos.IntToPosPlayer( _infos.PosPlayerToInt(_posPlayer)+2 %4 );
    return (_posPlayer == _infos.Taker()) || (teammate == _infos.Taker());
}
bool AIGameMemory::CardsFallen(CARDS_COLOR color,CARDS_HEIGHT height) const
{
    return _fallenCards.IsFallen(color,height);
}

bool AIGameMemory::HaveTen(CARDS_COLOR color) const
{
    Uint iColor = _infos.ColorToInt(color);
    return _HaveTen[iColor];
}

bool AIGameMemory::ProtectPoints(CARDS_COLOR color) const
{
    Uint iColor = _infos.ColorToInt(color);
    return _protectPoints[iColor];
}
Uint AIGameMemory::NbColorPlayed(CARDS_COLOR color) const //number of time each color have been played (first)
{
    Uint iColor = _infos.ColorToInt(color);
    return _nbColorPlayed[iColor];
}

CARDS_HEIGHT AIGameMemory::Greatest(CARDS_COLOR color) const //return the greatest cards I have in the color
{
    Uint iColor = _infos.ColorToInt(color);
    return _greatest[iColor];
}
CARDS_HEIGHT AIGameMemory::Smallest(CARDS_COLOR color) const //return the smallest cards I have in the color
{
    Uint iColor = _infos.ColorToInt(color);
    return _smallest[iColor];
}
