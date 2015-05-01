#ifndef IMAGES_MOVE_H
#define IMAGES_MOVE_H

/**
** This class defines properly a new kind of image :
** an image that can move.
**/

#include "Basic_Images.h"

class Images_Move : public Basic_Images
{
    public:
        Images_Move();
        Images_Move(std::string path,Uint32 current_time);
        virtual ~Images_Move();
        virtual void Update_Movement(Uint32 current_time); //for updating the movement
        void Set_animate(PositionGraphic& pos_end,Uint32 duration,Uint32 time_lag,Uint32 current_time); //the first 2 numbers are for the final position, the 3rd for the time in milisecond, the last one is for the time
    protected:
        void Move();
        PositionGraphic _pos_end;
        Uint32 _timer_end; //time of creation of the Basic_Images instance
        Uint32 _timer_current; //current time
        Uint32 _timer_start; //time of beginning of the movement
        Uint16 _fps;
        double _vx;
        double _vy;
        void setSpeed(Uint32 duration);

        virtual void act(); //to act : update the coordinates
    private:
};

#endif // IMAGES_MOVE_H
