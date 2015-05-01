#include "PositionGraphic.h"

void PositionGraphic::Set(Uint x, Uint y, POSITION_OPTION opt)
{
    _pos_x = x;
    _pos_y = y;
    _opt = opt;
}

void PositionGraphic::SetProperPosition(Uint width, Uint height)
{
    switch (_opt)
    {
        case CENTER : //center
        _pos_x -= width/2;
        _pos_y -= height/2;
        break;

        case TOP_LEFT : //high-left corner
        break;

        case TOP_CENTER : //high-center
        _pos_x -= width/2;
        break;

        case TOP_RIGHT : //high-right corner
        _pos_x -= width;
        break;

        case CENTER_RIGHT ://center-right
        _pos_x -= width;
        _pos_y -= height/2;
        break;

        case BOTTOM_RIGHT ://bottom-right
        _pos_x -= width;
        _pos_y -= height;
        break;

        case BOTTOM_CENTER : //bottom center
        _pos_x -= width/2;
        _pos_y -= height;
        break;

        case BOTTOM_LEFT : //bottom left
        _pos_y -= height;
        break ;

        case CENTER_LEFT : //center left
        _pos_y -= height/2;
        break ;

        default : //equivalent at top-left
        break;
    }
    _opt = TOP_LEFT; //default position for SDL
}

void PositionGraphic::Set(const PositionGraphic& pos)
{
    _opt = pos._opt;
    _pos_x = pos._pos_x;
    _pos_y = pos._pos_y;
}

PositionGraphic PositionGraphic::Shift(Uint number,const std::array<Uint,2>& pos) const
{
    PositionGraphic res(_pos_x+number*pos[0],_pos_y+number*pos[1],_opt);
    return res;
}
