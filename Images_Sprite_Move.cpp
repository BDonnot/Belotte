//#include "Images_Sprite_Move.h"
//using namespace::std;
//Images_Sprite_Move::Images_Sprite_Move()
//{
//    //ctor
//}
//Images_Sprite_Move::Images_Sprite_Move(std::string path,int number_sprite):
//Images_Sprite(path,number_sprite)
//,_timer_end(33)
//,_timer_current(33)
//,_timer_start(33)
//,_fps(32)
//,_vx(0.0)
//,_vy(0.0)
//{
//
//}
//Images_Sprite_Move::~Images_Sprite_Move()
//{
//    //dtor
//}
//void Images_Sprite_Move::Set_animate(PositionGraphic& pos_final,Uint32 duration,Uint32 time_lag)
//{
//    _timer_end = _timer_current+time_lag+duration;
//    _timer_start = _timer_current+time_lag;
//    pos_final.SetProperPosition(_width,_height);
//    _pos_end = pos_final;
//    setSpeed(duration);
//}
//void Images_Sprite_Move::setSpeed(Uint32 duration)
//{
//    _vx = static_cast<double>(_pos_end.Getx()) - static_cast<double>(_pos.Getx());
//    _vx /= static_cast<double>(duration);
//    _vy = static_cast<double>(_pos_end.Gety()) - static_cast<double>(_pos.Gety());
//    _vy /= static_cast<double>(duration);
//}
//void Images_Sprite_Move::Set_animate(PositionGraphic& pos_final)
//{
//    pos_final.SetProperPosition(_width,_height);
//    _pos_end = pos_final;
//    if(_timer_current <= _timer_start) //if the moving has not started
//    {
//        setSpeed(_timer_end - _timer_start);
//        return;
//    }
//    if (_timer_current <= _timer_end-_fps) //if it has not yet arrived
//    {
//        _timer_start = _info.Time();
//        setSpeed(_timer_end - _timer_start);
//    }
//    else //it has arrived at the wrong destination
//    {
//        _timer_start = _info.Time()-1;
//        _timer_end = _info.Time() + 200;
//        setSpeed(_timer_end - _timer_start);
//    }
//}
//void Images_Sprite_Move::Move()
//{
//    Uint timer_current = _info.Time();
//    if (timer_current <= _timer_start)
//    {
//        _timer_current = timer_current;
//        return; //If the mouvement has not started, we do nothing
//    }
//    if (timer_current > _timer_end)
//    {
//        _timer_current = timer_current;
//        _pos = _pos_end; //time is up, I am arrived, whatever append
//        return;
//    }
//    double dt = (static_cast<double>(timer_current) - static_cast<double>(_timer_current));
//    _timer_current = timer_current;
//    setSpeed(_timer_end - timer_current);
//    if (_vx > 0.0)  _pos.Getx() += min<Uint>(static_cast<Uint>(_vx*dt),_pos_end.Getx()-_pos.Getx());
//    else            _pos.Getx()-= min<Uint>(static_cast<Uint>((-_vx)*dt),_pos.Getx()-_pos_end.Getx());
//
//    if (_vy > 0.0)  _pos.Gety()+= min<Uint>(static_cast<Uint>(_vy*dt),_pos_end.Gety()-_pos.Gety());
//    else            _pos.Gety()-= min<Uint>(static_cast<Uint>((-_vy)*dt),_pos.Gety()-_pos_end.Gety());
//}
//void Images_Sprite_Move::act()
//{
//    Move();
//}
//void Images_Sprite_Move::SetPosition(PositionGraphic& pos_final) //suppose every move is over
//{
//    _height = _clip[_sprite_number].h;
//    _width = _clip[_sprite_number].w;
//    pos_final.SetProperPosition(_width,_height);
//    _pos = pos_final;
//    _pos_end = pos_final;
//}
