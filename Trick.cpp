#include "Trick.h"

using namespace std;
Trick::Trick()
{
    //ctor
}

Trick::~Trick()
{
}

Trick::Trick(Cards_Deck* pDeck,const std::array<Player*,4>& players,Uint windowsWidth,Uint windowsHeight,SDL_Event* pevent,SDL_Surface* pScreen):
Trick_Basic(pDeck,players),
//_info()
//,_pDeck(pDeck)
//,_players(players)
//,_currentTrick()
//,_previousTrick()
//,_cardsPlayedThisTurn(0)
//,_scoreTeam(_pScreen)
//_posTrick(),
//_oldFirstPlayer(GHOST),
_pScreen(pScreen),
_logo("images/sprite_trunk.png",pevent),
_teamName("fonts/MLSGY.ttf",20,{255,255,255},"???"),
_contract("fonts/MLSGY.ttf",20,{255,255,255},"???"),
_infoPreviousTrick("fonts/MLSGY.ttf",25,{255,255,255},"???"),
_ok(pevent,"OK",PositionGraphic(windowsWidth/2,windowsHeight - 225,CENTER)),
_posCard(),
_scoreTeam(pScreen)
{
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
            //_currentTrick[i]->Display(_pScreen);
            _currentTrick[i]->Display();
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
            //_previousTrick[i]->Display(_pScreen);
            _previousTrick[i]->Display();
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
    //_scoreTeam.Update();
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

void Trick::GatherCards()
{
    _scoreTeam.Update();
    posReturn(_info.FirstPlayer());
    for(Uint i = 0; i < 4; ++i)
    {
        if(_currentTrick[i] == nullptr) break; //TO DO : exception here !
        //_currentTrick[i]->InitMouvement(true,_posCard,600,0);
        _currentTrick[i]->InitMouvement(true,_posCard,600,0);
    }
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
