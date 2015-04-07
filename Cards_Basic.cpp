#include "Cards_Basic.h"
using namespace::std;

std::array<unsigned int,8> Cards_Basic::_value_trump = {0,0,14,10,20,3,4,11};
std::array<unsigned int,8> Cards_Basic::_value_no_trump = {0,0,0,10,2,3,4,11};

bool Cards_Basic::Win(const Card_Height& heightOther) const //
{
    Uint height_card = _height_card.ToInt();
    Uint height_other = heightOther.ToInt();
    //printf("My height is %d \n",height_card);
    //printf("The other height is %d\n",height_other);
    if (_infos.TrumpColor() == _color_card)
        return _matrix_win_trump[height_card][height_other];
    return _matrix_win_no_trump[height_card][height_other];
}
Cards_Basic::Cards_Basic(const Card_Height& height, const Card_Color& color):
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

const Card_Color& Cards_Basic::GetColour() const
{
    return _color_card;
}
const Card_Height& Cards_Basic::GetHeight() const
{
    return _height_card;
}

const Uint& Cards_Basic::Value() const
{
    if (_infos.TrumpColor() != _color_card)
        return _value_no_trump[_height_card.ToInt()];
    return _value_trump[_height_card.ToInt()];
}

std::string Cards_Basic::GetString() const
{
    string res = "<cards>";
    res += IntToString(_color_card.ToInt());
    res += ",";
    res += IntToString(_height_card.ToInt());
    res += "<\\cards>";
    return res;
}
