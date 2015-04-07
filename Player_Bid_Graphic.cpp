#include "Player_Bid_Graphic.h"

using namespace std;

DisplayOldBets::DisplayOldBets(SDL_Surface* screen
               ,Uint windows_width
               ,Uint windows_height
               ,Uint player_number
               ,Images_Text* text
               ,Images_Sprite<4>* logo):
_screen(screen)
,_pText(text)
,_pLogo(logo)
{
    bool vertical_move = (player_number%2==1);
    if (vertical_move)
    {
        _shift.at(0)= 0;
        _shift.at(1)= 60;
    }
    else
    {
        _shift.at(0)= 60;
        _shift.at(1)= 0;
    }
    Uint position[4][2] = {{windows_width/2-100,windows_height-230}
                            ,{windows_width-200,windows_height/2-135}
                            ,{windows_width/2 - 100,215}
                            ,{200,windows_height/2-135}};
    //POSITION_OPTION pos_opt[4] = {BOTTOM_CENTER,CENTER_RIGHT,TOP_CENTER,CENTER_LEFT};
    //PositionGraphic pos(position[player_number][0],position[player_number][1],pos_opt[player_number]);
    _pos_th.Set(position[player_number][0],position[player_number][1],CENTER);

}
void DisplayOldBets::Reset()
{
    _nb_bets = 0;
}
void DisplayOldBets::operator()(const Player_Bid& bid)
{
    //_pLogo->Change_sprite(bid.Color());
    string text;
    switch(bid.Color().Color())
    {
    case NOT_CHOSEN :
        return ;
    case NO :
        text = "passe";
        _pText->ChangeText(text);
        _pos_real = _pos_th.Shift(_nb_bets,_shift);
        _pText->SetPosition(_pos_real);
        _pText->Display(_screen);
        break;
    default :
        text = Player_Bid_Graphic::IntToString(bid.Color(),bid.Bid());
        _pLogo->ChangeSprite(bid.Color().ToInt());
        _pos_real = _pos_th.Shift(_nb_bets,_shift);
        _pLogo->SetPosition(_pos_real);
        _pLogo->Display(_screen);
        _pText->ChangeText(text);
        _pos_real = _pos_th.Shift(_nb_bets,_shift);
        _pText->SetPosition(_pos_real);
        _pText->Display(_screen);
        break;
    }
    _nb_bets++;
}
Uint DisplayOldBets::colorToInt(CARDS_COLOR color)
{
    switch(color)
    {
    case DIAMOND :
        return 0;
    case HEART :
        return 1;
    case SPADE :
        return 2;
    case CLUB :
        return 3;
    default :
        return 0;
    }
}


Player_Bid_Graphic::Player_Bid_Graphic()
{
    //ctor
}

Player_Bid_Graphic::~Player_Bid_Graphic()
{
    //dtor
}
Player_Bid_Graphic::Player_Bid_Graphic(Uint player_number,SDL_Event* pevent,SDL_Surface* screen,Uint windows_width,Uint windows_height):
Player_Bid()
,_bubble("images/sprite_bulles.png",pevent)
,_text("fonts/MLSGY.ttf",20,{255,255,255},"Passe")
,_color("images/sprite_take_color.png")
,_displayed_logo_color(true)
,_oldColor("images/sprite_take_color.png")
,_oldText("fonts/MLSGY.ttf",20,{255,255,255},"Passe")
,_dsplOldBets(screen,windows_width,windows_height,player_number,&_oldText,&_oldColor)
{
    Uint position[4][2] = {{windows_width/2+230,windows_height-150}
                            ,{windows_width,windows_height/2-150}
                            ,{windows_width/2 + 150,0}
                            ,{0,windows_height/2-150}};
    POSITION_OPTION pos_opt[4] = {BOTTOM_CENTER,CENTER_RIGHT,TOP_CENTER,CENTER_LEFT};
    PositionGraphic pos(position[player_number][0],position[player_number][1],pos_opt[player_number]);

    array<Uint,4> x = {0,200,400,600};
    array<Uint,4> y = {0,0,0,0};
    array<Uint,4> width = {160,160,160,160};
    array<Uint,4> height = {92,92,92,92};
    _bubble.SetDimensionSprite(x,y,width,height);
    _bubble.ChangeSprite(player_number);
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
string Player_Bid_Graphic::IntToString(const Card_Color & color,const Uint& number)
{
    string Result;
    if(color.Color()==NO)
    {
        Result = "Passe";
    }
    else
    {
        ostringstream convert;
        convert << number;
        Result = convert.str();
    }
    return Result;
}
void Player_Bid_Graphic::HandleText()
{
    string Result(IntToString(_color_bid,_bid));
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
//    string res = _listOldBet.front().GetString();
    return res;
}
