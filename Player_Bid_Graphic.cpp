#include "Player_Bid_Graphic.h"

using namespace std;

Player_Bid_Graphic::Player_Bid_Graphic(const Player_ID& player_number,SDL_Event* pevent,SDL_Surface* screen,Uint windows_width,Uint windows_height):
Player_Bid()
,_bubble("images/sprite_bulles.png",pevent)
,_text("fonts/MLSGY.ttf",20,255,255,255,"Passe")
,_color("images/sprite_take_color.png")
,_displayed_logo_color(true)
,_oldColor("images/sprite_take_color.png")
,_oldText("fonts/MLSGY.ttf",20,255,255,255,"Passe")
,_dsplOldBets(screen,windows_width,windows_height,player_number,&_oldText,&_oldColor)
{
    Uint position[4][2] = {{windows_width/2+230,windows_height-150}
                            ,{windows_width,windows_height/2-150}
                            ,{windows_width/2 + 150,0}
                            ,{0,windows_height/2-150}};
    POSITION_OPTION pos_opt[4] = {BOTTOM_CENTER,CENTER_RIGHT,TOP_CENTER,CENTER_LEFT};
    PositionGraphic pos(position[player_number.ToInt()][0],position[player_number.ToInt()][1],pos_opt[player_number.ToInt()]);

    array<Uint,4> x = {0,200,400,600};
    array<Uint,4> y = {0,0,0,0};
    array<Uint,4> width = {160,160,160,160};
    array<Uint,4> height = {92,92,92,92};
    _bubble.SetDimensionSprite(x,y,width,height);
    _bubble.ChangeSprite(player_number.ToInt());
    _bubble.SetPosition(pos);

    //positioning correctly the text and the images :
    PositionGraphic pos2(pos.Getx()+60,pos.Gety()+40,CENTER);
    _text.SetPosition(pos2);

    x = {1,64,129,197};
    width = {51,51,51,51};
    pos2.Set(pos.Getx()+120,pos.Gety()+40,CENTER);
    _color.SetDimensionSprite(x,y,width,width);
    _oldColor.SetDimensionSprite(x,y,width,width);
    _color.SetPosition(pos2);
    _color.ChangeSprite(1);
    _oldColor.ChangeSprite(1);

}

void Player_Bid_Graphic::HandleColor()
{
    switch(_color_bid.Color())
    {
    case DIAMOND :
        _color.ChangeSprite(0);
        _displayed_logo_color = true;
        return;
    case HEART :
        _color.ChangeSprite(1);
        _displayed_logo_color = true;
        return;
    case SPADE :
        _color.ChangeSprite(2);
        _displayed_logo_color = true;
        return;
    case CLUB :
        _color.ChangeSprite(3);
        _displayed_logo_color = true;
        return;
    default :
        _displayed_logo_color = false;
        return;
    }
}

void Player_Bid_Graphic::HandleText()
{
    string Result(_dsplOldBets.IntToString(_color_bid,_bid));
    _text.ChangeText(Result);
}

void Player_Bid_Graphic::handleGraphicsIfAny()
{
    HandleColor();
    HandleText();
}

void Player_Bid_Graphic::Display(SDL_Surface* screen,GAME_PHASES currentPhase)
{
    if(((currentPhase!=BIDDING)&&(currentPhase!=AFTER_BET))||(_color_bid==NOT_CHOSEN)) return;
    _bubble.Display(screen);
    _text.Display(screen);
    if(_displayed_logo_color) _color.Display(screen);
    if (_bubble.Mouse_Over())
    {
        _dsplOldBets.Reset();
        for_each(_listOldBet.begin(),_listOldBet.end(),_dsplOldBets);
    }
}

void Player_Bid_Graphic::Coinche()
{
    _displayed_logo_color = false;
    _text.ChangeText("Coinche");
}

void Player_Bid_Graphic::UpdateEvent()
{
    _bubble.Update_on_it();
}

bool Player_Bid_Graphic::AddBid(const Card_Color& color, Uint bid)
{
    _listOldBet.push_front(Player_Bid(static_cast<Player_Bid&>(*this)));
    if(Bid(color,bid)) return true;
    _listOldBet.pop_front();
    return false;
}

string Player_Bid_Graphic::GetString(Uint biddingTurn) const
{
    Uint sizeMax = _listOldBet.size();
    string res = "";
    if(biddingTurn < sizeMax)
    {
        auto itOldBet = _listOldBet.rbegin();
        advance(itOldBet,biddingTurn);
        res += (*itOldBet).GetString();
    }
    else if (biddingTurn == sizeMax) res += static_cast<const Player_Bid&>(*this).GetString();
    return res;
}
