#include "AIGameMemory.h"
using namespace std;

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
    Player_ID currentPlayer = _infos.FirstToPlay(posTrick,_posPlayer);
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
        currentPlayer.Next();
        currentColor = trick[i]->GetColour();
        if(currentColor != colorAsked)
        {
            _playerCut.SetCut(currentPlayer,colorAsked );
            if(trick[i]->Value() <= 4) _playerCalls.SetCut(currentPlayer,currentColor ); //we don't call with TEN, NINE or ACE ...
            //maybe a smarter option here
        }
    }
    //update smarter memory (for derived class)
    updateSmarter(trick,posTrick);
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
void AIGameMemory::updateEverythingElse(const Player_ID& firstToPlay)
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

    array< list<const Cards*> , 4 > _cardsPerColor;
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
        _cardsPerColor[iColor].push_back(pcard);
        if(pcard->Win(_greatest[iColor])) _greatest[iColor] = height;
        if(!(pcard->Win(_smallest[iColor]))) _smallest[iColor] = height;
        if(height == (color == _infos.TrumpColor() ? Card_Height(NINE) : Card_Height(TEN))) _HaveTen[iColor] = true;
    }
    //COMPUTE THE OTHER THINGS COLOR PER COLOR
    CARDS_COLOR colors[4] = {DIAMOND,HEART,SPADE,CLUB};
    for(Uint i = 0; i < 4; i++)
    {
        color = Card_Color( colors[i] );
        Uint iCol = color.ToInt();
        Uint nbLeft = 8-_fallenCards.ComputeFallen(color);
        if(_nbRemaining[iCol] == nbLeft)
        {
            Player_ID currentPlayer(firstToPlay);
            for(Uint j= 1; j < 4; j++)
            {
                currentPlayer.Next();
                _playerCut.SetCut(currentPlayer,color );
            }
        }
        if (_greatest[iCol] == _heightsMaster[iCol] )_IAmMaster[iCol] = true;
        computeScoreLongeAndProtectPoint(_cardsPerColor[iCol],_longe[iCol],_protectPoints[iCol],_IAmMaster[iCol],_greatest[iCol]);
    }

}
void AIGameMemory::computeScoreLongeAndProtectPoint(const std::list<const Cards*>& cardsInTheColor,Uint& scoreLonge, bool& protectPoints,bool iAmMasterColor,const Card_Height& greatest)
{
    //TO DO : we can perform some optimization here !
    bool isTrump = false;
    scoreLonge = 0;
    protectPoints = false;
    for(auto pcard : cardsInTheColor)
    {
        if((pcard->Value() >= 9) && (!iAmMasterColor)) protectPoints = true;
        if(!isTrump && (pcard->GetColour() == _infos.TrumpColor()) ) isTrump = true;
        continue;
    }
    if(iAmMasterColor)
    {
        Card_Height currentLonge(greatest);
        Card_Height tempHeight;
        Card_Color tempCol;
        Uint counter = 0;
        bool loopAgain = true;
        while(loopAgain && currentLonge.ToInt() > 0)
        {
            loopAgain = false;
            for(auto pcard : cardsInTheColor)
            {
                tempHeight = pcard->GetHeight();
                tempCol = pcard->GetColour();
                if((tempHeight == currentLonge) || (_fallenCards.IsFallen(tempCol,tempHeight)))
                {
                    counter++;
                    currentLonge = currentLonge.HeightUnder(isTrump);
                    loopAgain = true;
                }
            }
        }
        if(counter > 3) scoreLonge = 10;
        else if (counter > 2) scoreLonge = 5;
        else if (counter > 1) scoreLonge = 2;
        else scoreLonge = 0;
    }
}
bool AIGameMemory::callCut(const Player_ID& player,const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const
{
    return StoreCallCut.Cut(player,color);
}
bool AIGameMemory::opponentsCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const
{
    Player_ID opp1(_posPlayer.NextPlayer());
    Player_ID opp2 =  opp1.Teammate();
    return StoreCallCut.Cut(opp1.ToInt(),color) || StoreCallCut.Cut(opp2.ToInt(),color) ;
}
bool AIGameMemory::teammateCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const
{
    Player_ID teammate =  _posPlayer.Teammate();
    return StoreCallCut.Cut(teammate,color);
}
bool AIGameMemory::nextCallCut(const Card_Color& color,const MemorizeCutsCalls& StoreCallCut) const
{
    Player_ID nextPlayer(_posPlayer.NextPlayer());
    return StoreCallCut.Cut(nextPlayer,color) ;
}
bool AIGameMemory::Cut(const Player_ID& player,const Card_Color& color) const //Do 'player' cut at 'color'
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
bool AIGameMemory::Call(const Player_ID& player,const Card_Color& color) const //Do 'player' call at 'color'
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
    Player_ID teammate = _posPlayer.Teammate();
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
bool AIGameMemory::CanReceiveCard(const Player_ID& id, const Card_Color& col, const Card_Height& height)
{
    return !_playerCut.Cut(id,col) && canHave(id,col,height);
}
/*
bool AIGameMemory::CanHaveCard(const Player_ID& player,const Card_Color& color, const Card_Height& height) const
{
    return !_playerCut.Cut(player,color) && canHave(player,color,height);
}
*/
