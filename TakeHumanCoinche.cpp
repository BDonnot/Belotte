#include "TakeHumanCoinche.h"

using namespace std;


#if COMPLETE_GAME > 0
TakeHumanCoinche::TakeHumanCoinche(SDL_Event* event,SDL_Surface* screen,Uint16 windows_width, Uint16 windows_height):
        _windows_width(windows_width)
        ,_windows_height(windows_height)
        ,_screen(screen)
        ,_info()
        ,_carreau(event,move(PositionGraphic()),"images/sprite_take_color.png",0)
        ,_coeur(event,move(PositionGraphic()),"images/sprite_take_color.png",1)
        ,_pique(event,move(PositionGraphic()),"images/sprite_take_color.png",2)
        ,_trefle(event,move(PositionGraphic()),"images/sprite_take_color.png",3)
//        ,_tt_atout(event,move(PositionGraphic()),"images/sprite_take_color.png",3) //TO DO : when it will be available
//        ,_ss_atout(event,move(PositionGraphic()),"images/sprite_take_color.png",3) //TO DO : when it will be available
        ,_pass(event,"Passe",PositionGraphic())
        ,_ok(event,"OK",PositionGraphic())
        ,_plus(event,"+10",PositionGraphic())
        ,_minus(event,"-10",PositionGraphic())
        ,_score("fonts/MLSGY.ttf",30,255,255,255,"80")
        ,_unconfirmedBid()
        ,_confirmedBid()
        ,_displayed(true)
        ,_clock(0)
        ,_clock_min_before_new_change(0)
{
    PositionGraphic pos(windows_width/2,windows_height/3,CENTER);
    _score.SetPosition(pos);
    pos.Set(windows_width/2-81,windows_height/2,TOP_RIGHT);
    _carreau.SetPosition(pos);
    pos.Set(windows_width/2-10,windows_height/2,TOP_RIGHT);
    _coeur.SetPosition(pos);
    pos.Set(windows_width/2+10,windows_height/2,TOP_LEFT);
    _pique.SetPosition(pos);
    pos.Set(windows_width/2+81,windows_height/2,TOP_LEFT);
    _trefle.SetPosition(pos);
    pos.Set(2*windows_width/3,windows_height/2+70,TOP_CENTER);
    _pass.SetPosition(pos);
    pos.Set(windows_width/3,windows_height/2+70,TOP_CENTER);
    _ok.SetPosition(pos);
    pos.Set(windows_width/2 -50 ,windows_height/2-70,TOP_CENTER);
    _plus.SetPosition(pos);
    pos.Set(windows_width/2 + 50,windows_height/2-70,TOP_LEFT);
    _minus.SetPosition(pos);
    Reset(true);

}

void TakeHumanCoinche::Display(GAME_PHASES currentPhase)
{
    if(currentPhase != BIDDING)
        return ;
    Update();
    _carreau.Display(_screen);
    _coeur.Display(_screen);
    _pique.Display(_screen);
    _trefle.Display(_screen);
    //_tt_atout.Display(_screen);
    //_ss_atout.Display(_screen);
    _pass.Display(_screen);
    _ok.Display(_screen);
    _plus.Display(_screen);
    _minus.Display(_screen);
    _score.Display(_screen);
}
void TakeHumanCoinche::UpdateMouse(GAME_PHASES currentPhase)
{
    if(currentPhase != BIDDING)
        return ;
    _carreau.Update_on_it();
    _coeur.Update_on_it();
    _pique.Update_on_it();
    _trefle.Update_on_it();
    //_tt_atout.Update_on_it();
    //_ss_atout.Update_on_it();
    _pass.Update_on_it();
    _ok.Update_on_it();
    _plus.Update_on_it();
    _minus.Update_on_it();
}
void TakeHumanCoinche::Update()
{
    if(_info.Time() < _clock_min_before_new_change)
    {
         return;
    }
    Uint Number = max<Uint>(_info.MaxBid(),_unconfirmedBid.Bid());
    Uint minBet = max<Uint>(_info.MaxBid(),80);

    if(_plus.Click(true))
    {
        _clock_min_before_new_change = _info.Time() + LAG_MIN;
        string text = _score.GetText();
        (Number < 270 ) ? Number += 10 : Number = 270;
        _unconfirmedBid.Bid(Number);
    }
    else if(_minus.Click(true))
    {
        _clock_min_before_new_change = _info.Time() + LAG_MIN;
        string text = _score.GetText();
        (Number > minBet + 10 ) ? Number -= 10 : Number = minBet;
        _unconfirmedBid.Bid(Number);
    }
    if (_coeur.Click(false)) _unconfirmedBid.Bid(HEART,Number);
    if (_carreau.Click(false)) _unconfirmedBid.Bid(DIAMOND,Number);
    if (_pique.Click(false)) _unconfirmedBid.Bid(SPADE,Number);
    if (_trefle.Click(false)) _unconfirmedBid.Bid(CLUB,Number);
    ChangeText(Number);
}

void TakeHumanCoinche::ChangeText(Uint number)
{
    _score.ChangeText(IntToString(number));
}

const Player_Bid& TakeHumanCoinche::Has_Taken()
{
    if (_pass.Click(true)) _confirmedBid.Bid(NO,_info.MaxBid());
    if (_ok.Click(true)) _confirmedBid.Bid(_unconfirmedBid.Color(),_unconfirmedBid.Bid());
    return _confirmedBid;
}

void TakeHumanCoinche::UpdateClock(Uint32 clock)
{
    _clock = clock;
}

void TakeHumanCoinche::Reset(bool reset_unconfirmedBid)
{
    _confirmedBid.Bid(NOT_CHOSEN,80);
    if(reset_unconfirmedBid) _unconfirmedBid.Bid(NOT_CHOSEN,80);
    _plus.Reset_Click();
    _minus.Reset_Click();
    _coeur.Reset_Click();
    _carreau.Reset_Click();
    _pique.Reset_Click();
    _trefle.Reset_Click();
    _pass.Reset_Click();
    _ok.Reset_Click();
}
#endif //#if COMPLETE_GAME > 0