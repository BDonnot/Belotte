#ifndef IMAGES_SPRITE_MOVE_H
#define IMAGES_SPRITE_MOVE_H

/**
** This class define an specific type of image :
** with multiple faces, and that can move.
**/

#include "Basic_Game_Info.h"
#include "Images_Sprite.h"

template<Uint numberOfSprite>
class Images_Sprite_Move : public Images_Sprite<numberOfSprite>
{
    protected :
        PositionGraphic _pos_end;
        Uint32 _timer_end; //time of creation of the Basic_Images instance
        Uint32 _timer_current; //current time
        Uint32 _timer_start; //time of beginning of the movement
        Uint16 _fps;
        double _vx;
        double _vy;
        void setSpeed(Uint32 duration);

    public:
        Images_Sprite_Move(){}
        Images_Sprite_Move(std::string path);
        virtual ~Images_Sprite_Move(){}
        virtual void SetPosition(PositionGraphic& pos);
        void Set_animate(PositionGraphic& pos_final,Uint32 duration,Uint32 time_lag); //the first 2 numbers are for the final position, the 3rd for the time in milisecond, the last one is for the time
        void Set_animate(PositionGraphic& pos_final); //init the mouvement without updating the current time

    protected:
        void Move();
        virtual void act(); //to act : update the coordonatess

    private:
};

template<Uint numberOfSprite>
Images_Sprite_Move<numberOfSprite>::Images_Sprite_Move(std::string path):
Images_Sprite<numberOfSprite>(path)
,_timer_end(33)
,_timer_current(33)
,_timer_start(33)
,_fps(32)
,_vx(0.0)
,_vy(0.0)
{
    this->ChangeSprite(0);
}

template<Uint numberOfSprite>
void Images_Sprite_Move<numberOfSprite>::Set_animate(PositionGraphic& pos_final,Uint32 duration,Uint32 time_lag)
{
    _timer_end = _timer_current+time_lag+duration;
    _timer_start = _timer_current+time_lag;
    pos_final.SetProperPosition(this->_width
                                ,this->_height);
    _pos_end = pos_final;
    setSpeed(duration);
}

template<Uint numberOfSprite>
void Images_Sprite_Move<numberOfSprite>::setSpeed(Uint32 duration)
{
    PositionGraphic& pos = (this)->_pos;
    PositionGraphic& pos_end = (this)->_pos_end;
    _vx = static_cast<double>(pos_end.Getx()) - static_cast<double>(pos.Getx());
    _vx /= static_cast<double>(duration);
    _vy = static_cast<double>(pos_end.Gety()) - static_cast<double>(pos.Gety());
    _vy /= static_cast<double>(duration);
}

template<Uint numberOfSprite>
void Images_Sprite_Move<numberOfSprite>::Set_animate(PositionGraphic& pos_final)
{
    const Basic_Game_Info& info = (this)->_info;
    pos_final.SetProperPosition((this)->_width
                                ,(this)->_height);
    (this)->_pos_end = pos_final;
    if(_timer_current <= _timer_start) //if the moving has not started
    {
        setSpeed(_timer_end - _timer_start);
        return;
    }
    if (_timer_current <= _timer_end-_fps) //if it has not yet arrived
    {
        _timer_start = info.Time();
        setSpeed(_timer_end - _timer_start);
    }
    else //it has arrived at the wrong destination
    {
        _timer_start = info.Time()-1;
        _timer_end = info.Time() + 200;
        setSpeed(_timer_end - _timer_start);
    }
}

template<Uint numberOfSprite>
void Images_Sprite_Move<numberOfSprite>::Move()
{
    PositionGraphic& pos = (this)->_pos;
    PositionGraphic& pos_end = (this)->_pos_end;
    Uint timer_current = (this)->_info.Time();
    if (timer_current <= _timer_start)
    {
        _timer_current = timer_current;
        return; //If the movement has not started, we do nothing
    }
    if (timer_current > _timer_end)
    {
        _timer_current = timer_current;
        pos = pos_end; //time is up, I am arrived, whatever append
        return;
    }
    double dt = (static_cast<double>(timer_current) - static_cast<double>(_timer_current));
    _timer_current = timer_current;
    setSpeed(_timer_end - timer_current);
    if (_vx > 0.0)  pos.Getx() += std::min<Uint>(static_cast<Uint>(_vx*dt),pos_end.Getx()-pos.Getx());
    else            pos.Getx() -= std::min<Uint>(static_cast<Uint>((-_vx)*dt),pos.Getx()-pos_end.Getx());

    if (_vy > 0.0)  pos.Gety() += std::min<Uint>(static_cast<Uint>(_vy*dt),pos_end.Gety()-pos.Gety());
    else            pos.Gety() -= std::min<Uint>(static_cast<Uint>((-_vy)*dt),pos.Gety()-pos_end.Gety());
}

template<Uint numberOfSprite>
void Images_Sprite_Move<numberOfSprite>::act()
{
    Move();
}

template<Uint numberOfSprite>
void Images_Sprite_Move<numberOfSprite>::SetPosition(PositionGraphic& pos_final) //suppose every move is over
{
    this->_height = this->_clip[this->_sprite_number].h;
    this->_width = this->_clip[this->_sprite_number].w;
    pos_final.SetProperPosition(this->_width,this->_height);
    this->_pos = pos_final;
    this->_pos_end = pos_final;
}
#endif // IMAGES_SPRITE_MOVE_H
