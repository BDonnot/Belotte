#include "Images_Move.h"
using namespace::std;

Images_Move::Images_Move()
{
    //ctor
}
Images_Move::Images_Move(std::string path,Uint32 current_time):Basic_Images(path)
{
    _timer_end = current_time;
    _timer_current = current_time;
    _timer_start = current_time;
    _fps =1000/32;
}
Images_Move::~Images_Move()
{
    //dtor
}
void Images_Move::Set_animate(PositionGraphic& pos_end,Uint32 duration,Uint32 time_lag,Uint32 current_time)
{
    _timer_current = current_time;
    _timer_end = current_time+time_lag+duration;
    _timer_start = current_time+time_lag;
//    _pos_option = option;
//    std::array<Uint16,2> pos_f = position_image_aux();
//    _end_x = end_x_th + pos_f[0];
//    _end_y = end_y_th + pos_f[1];
    pos_end.SetProperPosition(_width,_height);
    _pos_end = _pos_end;
}
void Images_Move::Move()
{
   Uint timer_current = _info.Time();
    if (timer_current <= _timer_start)
    {
        _timer_current = timer_current;
        return; //If the mouvement has not started, we do nothing
    }
    if (timer_current > _timer_end)
    {
        _timer_current = timer_current;
        _pos = _pos_end; //time is up, I am arrived, whatever append
        return;
    }
    double dt = (static_cast<double>(timer_current) - static_cast<double>(_timer_current));
    _timer_current = timer_current;
    setSpeed(_timer_end - timer_current);
    if (_vx > 0.0)  _pos.Getx() += min<Uint>(static_cast<Uint>(_vx*dt),_pos_end.Getx()-_pos.Getx());
    else            _pos.Getx()-= min<Uint>(static_cast<Uint>((-_vx)*dt),_pos.Getx()-_pos_end.Getx());

    if (_vy > 0.0)  _pos.Gety()+= min<Uint>(static_cast<Uint>(_vy*dt),_pos_end.Gety()-_pos.Gety());
    else            _pos.Gety()-= min<Uint>(static_cast<Uint>((-_vy)*dt),_pos.Gety()-_pos_end.Gety());
}
void Images_Move::act()
{
    Move();
}
void Images_Move::Update_Movement(Uint32 current_time)
{
    _timer_current = current_time;
}
void Images_Move::setSpeed(Uint32 duration)
{
    _vx = static_cast<double>(_pos_end.Getx()) - static_cast<double>(_pos.Getx());
    _vx /= static_cast<double>(duration);
    _vy = static_cast<double>(_pos_end.Gety()) - static_cast<double>(_pos.Gety());
    _vy /= static_cast<double>(duration);
}
