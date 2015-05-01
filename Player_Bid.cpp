#include "Player_Bid.h"
using namespace std;

bool Player_Bid::Bid(const Card_Color& color,Uint bid)
{
    Uint minbid = _info.MaxBid() == MINBET ? MINBET+1 : _info.MaxBid(); //TO ENSURE THAT WE BET AT LEAST MINBET+1
    switch (color.Color())
    {
    case NOT_CHOSEN :
        _color_bid = NOT_CHOSEN;
        _bid = max<Uint>(bid,minbid);
        return false;
    case NO :
        _color_bid = NO;
        _bid = minbid;
        handleGraphicsIfAny();
        return true;
    default :
        if(bid <= _info.MaxBid())
        {
            _color_bid = NOT_CHOSEN;
            _bid = minbid;
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
bool Player_Bid::Bid(const Card_Color& color)
{
    return Bid(color,_bid);
}
void Player_Bid::handleGraphicsIfAny()
{

}
void Player_Bid::Reset()
{
    _color_bid = NOT_CHOSEN;
    _bid = max<Uint>(MINBET,_info.MaxBid());
}

std::string Player_Bid::GetString() const
{
    string res = "";
    if(_color_bid==NOT_CHOSEN) return res;
    res += "<bid>";
    switch(_color_bid.Color())
    {
        case HEART : case CLUB : case SPADE : case DIAMOND :
            res += IntToString(_color_bid.ToInt());
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
