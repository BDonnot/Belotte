#include "Trick.h"

using namespace std;
Trick::Trick()
{
    //ctor
}

Trick::~Trick()
{
    for(Uint i = 0;i < 4; ++i)
    {
        delete _currentTrick[i];
        delete _previousTrick[i];
    }
}

Trick::Trick(Cards_Deck* pDeck,const std::array<Player*,4>& players,Uint windowsWidth,Uint windowsHeight,SDL_Event* pevent,SDL_Surface* pScreen):
_info()
,_pDeck(pDeck)
,_players(players)
,_currentTrick()
,_previousTrick()
,_posTrick()
,_pScreen(pScreen)
,_logo("images/sprite_trunk.png",pevent)
,_teamName("fonts/MLSGY.ttf",20,{255,255,255},"???")
,_contract("fonts/MLSGY.ttf",20,{255,255,255},"???")
,_infoPreviousTrick("fonts/MLSGY.ttf",25,{255,255,255},"???")
,_ok(pevent,"OK",PositionGraphic(windowsWidth/2,windowsHeight - 225,CENTER))
,_cardsPlayedThisTurn(0)
,_posCard()
,_scoreTeam(_pScreen)
,_oldFirstPlayer(GHOST)
{
    _currentTrick = {nullptr,nullptr,nullptr,nullptr};
    _previousTrick = {nullptr,nullptr,nullptr,nullptr};
    array<Uint,4> x = {0,150,0,150};
    array<Uint,4> y = {0,0,150,150};
    array<Uint,4> size_ = {100,100,100,100};
    _logo.SetDimensionSprite(x,y,size_,size_);
    _logo.ChangeSprite(0);
    PositionGraphic pos(0,0,TOP_LEFT);
    _logo.SetPosition(pos);
    pos.Set(50,30,CENTER);
    _teamName.SetPosition(pos);
    pos.Set(50,70,CENTER);
    _contract.SetPosition(pos);
    _infoPreviousTrick.ChangeText("Voici le dernier pli :");
    pos.Set(windowsWidth/2,windowsHeight/3-20,CENTER);
    _infoPreviousTrick.SetPosition(pos);
    _posTrick = {FIRST,SECOND,THIRD,FOURTH};
}
void Trick::SetPlayers(const array<Player*,4>& players)
{
    _players = players;
}
void Trick::Display(GAME_PHASES currentPhase)
{
    if ((currentPhase != PLAYING)&&(currentPhase != AFTER_TRICK0)&&(currentPhase != AFTER_TRICK1)) return;
    _scoreTeam.Display(currentPhase);
    _logo.Display(_pScreen);
    _teamName.Display(_pScreen);
    _contract.Display(_pScreen);
    if(!_logo.Mouse_Over())
    {
        for(Uint i = 0; i < 4; ++i)
        {
            if(_currentTrick[i]==nullptr) break;
            _currentTrick[i]->Display(_pScreen);
        }
    }
    else
    {
        _infoPreviousTrick.Display(_pScreen);
        PLAYER_ID posPlayer = _oldFirstPlayer;
         for(Uint i = 0; i < 4; ++i)
        {
            if(_previousTrick[i]==nullptr) break;
            updatePosCard(posPlayer);
            _previousTrick[i]->SetPosition(_posCard);
            _previousTrick[i]->ChangeSprite(1);
            posPlayer = _info.Next(posPlayer);
            _previousTrick[i]->Display(_pScreen);
        }
    }
    if ((_currentTrick.back() != nullptr)&&(currentPhase == PLAYING)) _ok.Display(_pScreen);
}
void Trick::UpdateEvent(GAME_PHASES currentPhase)
{
    if ((currentPhase != PLAYING)&&(currentPhase != AFTER_TRICK0)&&(currentPhase != AFTER_TRICK1)) return;
    _logo.Update_on_it();
    _ok.Update_on_it();
}
void Trick::Update()
{
    switch(_info.TrumpColor())
    {
    case DIAMOND :
        _logo.ChangeSprite(0);
        break;
    case HEART :
        _logo.ChangeSprite(1);
        break;
    case SPADE :
        _logo.ChangeSprite(2);
        break;
    case CLUB :
        _logo.ChangeSprite(3);
        break;
    default :
        _logo.ChangeSprite(0);
        break;
    }
    _contract.ChangeText(Player_Bid_Graphic::IntToString(_info.TrumpColor(),_info.MaxBid()));
    string text = "Equipe ";
    text +=Player_Bid_Graphic::IntToString(_info.TrumpColor(),_info.Taker()%2+1);
    _teamName.ChangeText(text);
}
bool Trick::Play() // TO DO code the same way as Bidding.cpp
{
    Uint first = _info.FirstPlayer();
    Cards* pcard = nullptr;
    Uint counter = 0;
    _scoreTeam.Update();
    for (Uint i = first; i < first+4; ++i, ++counter)
    {
        if(_currentTrick[counter] != nullptr) continue;
        pcard = _players[i%4]->PlayCard(_currentTrick);
        if(pcard == nullptr) break; //the player has not chosen yet
        _currentTrick[counter] = pcard;
        updatePosCard(i%4);
        //pcard->InitMouvement(false,_posCard,1000,(i%4)*50);
        pcard->InitMouvement(pcard->GetSpriteNumber() == 0,_posCard,1000,(i%4)*50);
        POSITION_TRICK winner = whoWinTrick();
        _info.SetHigherCard(winner);
        _info.SetNumberCardsPlayed();
        break;

    }
    if((_currentTrick.back() != nullptr)&&(_ok.Click(true)))
    {
        for (Uint i = first; i < first+4; ++i, ++counter)
        {
            _players[i%4]->updateEndTrick(_currentTrick,_info.IntToPosTrick(i));
        }
        _oldFirstPlayer = _info.FirstPlayer();
        POSITION_TRICK winner = whoWinTrick();
        array<Uint,4> values;
        for (Uint i = 0; i < 4; ++i)
        {
            values[i] = _currentTrick[i]->Value();
        }
        _info.SetScores(winner,values);
//        _scoreTeam.Update();
        return true;
    }
    return false;
}
void Trick::updatePosCard(Uint player)
{
    switch(player)
    {
        case 0 :
            _posCard.Set(_info.WindowsWidth()/2, _info.WindowsHeight()/2+80,CENTER);
            return;
        case 1 :
            _posCard.Set(_info.WindowsWidth()/2 +55, _info.WindowsHeight()/2,CENTER);
            return;
        case 2 :
            _posCard.Set(_info.WindowsWidth()/2, _info.WindowsHeight()/2-80,CENTER);
            return;
        case 3 :
            _posCard.Set(_info.WindowsWidth()/2-55, _info.WindowsHeight()/2,CENTER);
            return;
        default :
            _posCard.Set(0,0,TOP_LEFT);
            return;
    }
}
POSITION_TRICK Trick::whoWinTrick()
{
    POSITION_TRICK res = FIRST;
    for(Uint i = 1; i <4; i++)
    {
        if(_currentTrick[i]==nullptr) break;
        if(_currentTrick[i]->GetColour()==_currentTrick[res]->GetColour())
        {
            if(_currentTrick[i]->Win(_currentTrick[res]->GetHeight())) res = _posTrick[i];
        }
        else
        {
            if(_currentTrick[i]->GetColour()==_info.TrumpColor()) res = _posTrick[i];
        }
    }
    return res;
}
void Trick::GatherCards()
{
    posReturn(_info.FirstPlayer());
    for(Uint i = 0; i < 4; ++i)
    {
        if(_currentTrick[i] == nullptr) break;
        //_currentTrick[i]->InitMouvement(true,_posCard,600,0);
        _currentTrick[i]->InitMouvement(true,_posCard,600,0);
    }
}
void Trick::WinnerTakeCards()
{
    for(Uint i = 0; i < 4; ++i)
    {
        if(_previousTrick[i] == nullptr) break;
        _pDeck->GetCardBack(_previousTrick[i]);
    }
    if(_info.TrickNumber()==8)
    {
        for(Uint i = 0; i < 4; ++i)
        {
            if(_currentTrick[i] == nullptr) break;
            _pDeck->GetCardBack(_currentTrick[i]);
        }
    }
    _previousTrick = _currentTrick;
    _currentTrick = {nullptr,nullptr,nullptr,nullptr};
}
void Trick::posReturn(PLAYER_ID posPlayer)
{
    switch(posPlayer)
    {
        case PLAYER0 :
            _posCard.Set(_info.WindowsWidth()/2,_info.WindowsHeight()-120,CENTER);
            return;
        case PLAYER1 :
            _posCard.Set(_info.WindowsWidth() -100,_info.WindowsHeight()/2,CENTER);
            return;
        case PLAYER2 :
            _posCard.Set(_info.WindowsWidth()/2,120,CENTER);
            return;
        case PLAYER3 :
            _posCard.Set(100,_info.WindowsHeight()/2,CENTER);
            return;
        default : //TO DO exception here
            _posCard.Set(0,0,TOP_LEFT);
            return;
    }
}
string Trick::GetString() const
{
    string res = "<trick : ";
    res += IntToString(_info.TrickNumber());
    res += ">";
    Uint first = _oldFirstPlayer;
    Uint counter = 0;
    for(Uint i = first; i < first+4; ++i, ++counter)
    {
        res += _players[i%4]->GetString(_currentTrick[counter]->GetString());
    }
    res += "<\\trick>";
    return res;
}
