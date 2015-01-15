#include "Player_Bid.h"
using namespace std;

Player_Bid::Player_Bid():
_color_bid(NOT_CHOSEN)
,_bid(80)
{
    //ctor
}

Player_Bid::~Player_Bid()
{
    //dtor
}
const CARDS_COLOR Player_Bid::Color() const
{
    return _color_bid;
}
const Uint Player_Bid::Bid() const
{
    return _bid;
}
bool Player_Bid::Bid(CARDS_COLOR color,Uint bid)
{
    switch (color)
    {
    case NOT_CHOSEN :
        _color_bid = NOT_CHOSEN;
        _bid = max<Uint>(bid,_info.MaxBid());
        //handleGraphicsIfAny();
        return false;
    case NO :
        _color_bid = NO;
        _bid = _info.MaxBid();
        handleGraphicsIfAny();
        return true;
    default :
        if(bid <= _info.MaxBid())
        {
            _color_bid = NOT_CHOSEN;
            _bid = _info.MaxBid();
            return false;
        }
        _color_bid = color;
        _bid = bid;
        handleGraphicsIfAny();
        return true;
    }
}
bool Player_Bid::Bid(Uint bid)
{
    return Bid(_color_bid,bid);
}
bool Player_Bid::Bid(CARDS_COLOR color)
{
    return Bid(color,_bid);
}
void Player_Bid::handleGraphicsIfAny()
{

}
void Player_Bid::Reset()
{
    _color_bid = NOT_CHOSEN;
    _bid = max<Uint>(80,_info.MaxBid());
    //_displayed_logo_color = true;
}
 const CARDS_COLOR& Player_Bid::GetColorBid() const
 {
     return _color_bid;
 }

 std::string Player_Bid::GetString() const
 {
    string res = "";
    if(_color_bid==NOT_CHOSEN) return res;
    res += "<bid>";
    switch(_color_bid)
    {
        case HEART : case CLUB : case SPADE : case DIAMOND :
            res += IntToString(_color_bid);
            res += ",";
            res += IntToString(_bid);
        break;
        case NO :
            res += "6,0";
            break;
        default :
            break;
    }
    res += "<\\bid>";
    return res;
 }
