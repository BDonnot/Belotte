#include "Trick.h"

using namespace std;

Trick::Trick(Cards_Deck* pDeck,const std::array<Player*,4>& players,Uint windowsWidth,Uint windowsHeight,SDL_Event* pevent,SDL_Surface* pScreen):
Trick_Basic(pDeck,players),
_pScreen(pScreen),
_logo("images/sprite_trunk.png",pevent),
_teamName("fonts/MLSGY.ttf",20,255,255,255,"???"),
_contract("fonts/MLSGY.ttf",20,255,255,255,"???"),
_infoPreviousTrick("fonts/MLSGY.ttf",25,255,255,255,"???"),
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
        for(Uint i = 0; i < 4 ; ++i) //TO DO : improve for the times GatherCards et WinnertakCards ...
        {
            if(_currentTrick[i]==nullptr) break;
            _currentTrick[i]->Display();
        }
    }
    else
    {
        _infoPreviousTrick.Display(_pScreen);
        Player_ID posPlayer(_oldFirstPlayer);
        for(Uint i = 0; i < 4; ++i)
        {
            if(_previousTrick[i]==nullptr) break;
            updatePosCard(posPlayer);
            _previousTrick[i]->SetPosition(_posCard);
            _previousTrick[i]->ChangeSprite(1);
            posPlayer.Next();
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
    switch(_infos.TrumpColor().Color())
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
    _contract.ChangeText(DisplayOldBets::IntToString(_infos.TrumpColor(),_infos.MaxBid()));
    string text = "Equipe ";
	text += DisplayOldBets::IntToString(_infos.TrumpColor(), _infos.Taker().ToInt() % 2 + 1);
    _teamName.ChangeText(text);
    Reset();
}

void Trick::updatePosCard(const Player_ID& player)
{
    switch(player.ID())
    {
        case PLAYER0 :
            _posCard.Set(_infos.WindowsWidth()/2, _infos.WindowsHeight()/2+80,CENTER);
            return;
        case PLAYER1 :
            _posCard.Set(_infos.WindowsWidth()/2 +55, _infos.WindowsHeight()/2,CENTER);
            return;
        case PLAYER2 :
            _posCard.Set(_infos.WindowsWidth()/2, _infos.WindowsHeight()/2-80,CENTER);
            return;
        case PLAYER3 :
            _posCard.Set(_infos.WindowsWidth()/2-55, _infos.WindowsHeight()/2,CENTER);
            return;
        default :
            _posCard.Set(0,0,TOP_LEFT);
            return;
    }
}

void Trick::GatherCards()
{
    _scoreTeam.Update();
    posReturn(_to_play);
    for(Uint i = 0; i < 4; ++i)
    {
        //_currentTrick[i]->InitMouvement(true,_posCard,1000,(i%4)*50);
        _currentTrick[i]->HideCard(_posCard,600,0); //hide the cards
    }
}

void Trick::posReturn(const Player_ID& posPlayer)
{
    switch(posPlayer.ID())
    {
        case PLAYER0 :
            _posCard.Set(_infos.WindowsWidth()/2,_infos.WindowsHeight()-120,CENTER);
            return;
        case PLAYER1 :
            _posCard.Set(_infos.WindowsWidth() -100,_infos.WindowsHeight()/2,CENTER);
            return;
        case PLAYER2 :
            _posCard.Set(_infos.WindowsWidth()/2,120,CENTER);
            return;
        case PLAYER3 :
            _posCard.Set(100,_infos.WindowsHeight()/2,CENTER);
            return;
        default : //TO DO exception here
            _posCard.Set(0,0,TOP_LEFT);
            return;
    }
}
