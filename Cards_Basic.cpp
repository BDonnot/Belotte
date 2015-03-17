#include "Cards_Basic.h"
using namespace::std;

std::array<unsigned int,8> Cards_Basic::_value_trump = {0,0,14,10,20,3,4,11};
std::array<unsigned int,8> Cards_Basic::_value_no_trump = {0,0,0,10,2,3,4,11};

bool Cards_Basic::Win(CARDS_HEIGHT heightOther) const //
{
    Uint height_card = _infos.HeightToInt(_height_card);
    Uint height_other = _infos.HeightToInt(heightOther);
    if (_infos.TrumpColor()==_color_card)
        return _matrix_win_trump[height_card][height_other];
    return _matrix_win_no_trump[height_card][height_other];
}
/*
Uint Cards_Basic::HeightToInt(CARDS_HEIGHT height_card) const
{
    switch(height_card)
    {
        case SEVEN :
            return 0;
        case EIGHT :
            return 1;
        case NINE :
            return 2;
        case TEN :
            return 3;
        case JACK :
            return 4;
        case QUEEN :
            return 5;
        case KING :
            return 6;
        case ACE :
            return 7;
        default :
            return 0;
    }
}
*/
Cards_Basic::Cards_Basic(CARDS_HEIGHT height, CARDS_COLOR color):
_height_card(height),
_color_card(color)
{
    _matrix_win_trump[0] = {false,false,false,false,false,false,false,false};
    _matrix_win_trump[1] = {true,false,false,false,false,false,false,false};
    _matrix_win_trump[2] = {true,true,false,true,false,true,true,true};
    _matrix_win_trump[3] = {true,true,false,false,false,true,true,false};
    _matrix_win_trump[4] = {true,true,true,true,false,true,true,true};
    _matrix_win_trump[5] = {true,true,false,false,false,false,false,false};
    _matrix_win_trump[6] = {true,true,false,false,false,true,false,false};
    _matrix_win_trump[7] = {true,true,false,true,false,true,true,false};

    _matrix_win_no_trump[0] = {false,false,false,false,false,false,false,false};
    _matrix_win_no_trump[1] = {true,false,false,false,false,false,false,false};
    _matrix_win_no_trump[2] = {true,true,false,false,false,false,false,false};
    _matrix_win_no_trump[3] = {true,true,true,false,true,true,true,false};
    _matrix_win_no_trump[4] = {true,true,true,false,false,false,false,false};
    _matrix_win_no_trump[5] = {true,true,true,false,true,false,false,false};
    _matrix_win_no_trump[6] = {true,true,true,false,true,true,false,false};
    _matrix_win_no_trump[7] = {true,true,true,true,true,true,true,false};
}

const CARDS_COLOR& Cards_Basic::GetColour() const
{
    return _color_card;
}
const CARDS_HEIGHT& Cards_Basic::GetHeight() const
{
    return _height_card;
}

const Uint& Cards_Basic::Value() const
{
    if (_infos.TrumpColor() != _color_card)
        return _value_no_trump[_height_card];
    return _value_trump[_height_card];
}

std::string Cards_Basic::GetString() const
{
    string res = "<cards>";
    res += IntToString(_color_card);
    res += ",";
    res += IntToString(_height_card);
    res += "<\\cards>";
    return res;
}
