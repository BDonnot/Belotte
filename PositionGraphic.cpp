#include "PositionGraphic.h"

PositionGraphic::PositionGraphic():
_pos_x(0)
,_pos_y(0)
,_opt(CENTER)
{
    //ctor
}
PositionGraphic::PositionGraphic(Uint x, Uint y, POSITION_OPTION opt):
_pos_x(x)
,_pos_y(y)
,_opt(opt)
{
}

PositionGraphic::~PositionGraphic()
{
    //dtor
}
const Uint& PositionGraphic::Getx() const
{
    return _pos_x;
}
const Uint& PositionGraphic::Gety()const
{
    return _pos_y;
}
const POSITION_OPTION& PositionGraphic::GetOpt() const
{
    return _opt;
}
Uint& PositionGraphic::Getx()
{
    return _pos_x;
}
Uint& PositionGraphic::Gety()
{
    return _pos_y;
}
void PositionGraphic::Set(Uint x, Uint y, POSITION_OPTION opt)
{
    _pos_x = x;
    _pos_y = y;
    _opt = opt;
}
void PositionGraphic::Setx(Uint x)
{
    _pos_x = x;
}
void PositionGraphic::Sety(Uint y)
{
    _pos_y = y;
}
void PositionGraphic::SetOpt(POSITION_OPTION opt)
{
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
        //_pos_x = 0;
        //res_y = 0;
        //nothing to do
        break;

        case TOP_CENTER : //high-center
        _pos_x -= width/2;
        //_pos_y = 0;
        break;

        case TOP_RIGHT : //high-right corner
        _pos_x -= width;
        //_pos_y = 0;
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
        //_pos_x = 0;
        _pos_y -= height;
        break ;

        case CENTER_LEFT : //center left
        //_pos_x = 0 ;
        _pos_y -= height/2;
        break ;

        default : //equivalent at top-left
        //_pos_x = 0;
        //_pos_y = 0 ;
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
bool PositionGraphic::operator==(const PositionGraphic& pos) const
{
    return (_pos_x==pos._pos_x) && (_pos_y == pos._pos_y);
}
