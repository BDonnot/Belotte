#ifndef BASIC_TRANSPARENCY_H
#define BASIC_TRANSPARENCY_H

#include "SDL/SDL.h"

class Basic_Transparency
{
    protected:
        double _alpha; //level of opacity of the image
        double _alpha_end; //the level at the end of the modification
        Uint32 _timer_alpha_end; //when this timer is reach, alpha doesn't change anymore
        Uint32 _timer_alpha_current; //to represent the current time
        Uint32 _timer_alpha_start; //timer at which alpha's changing started/will start
        Uint16 _fps;
    public:
        Basic_Transparency();
        Basic_Transparency(Uint8 transparency);
        virtual ~Basic_Transparency();
        void Set_Transparent(bool increasing,Uint32 duration,Uint32 time_lag,Uint32 current_time); //to start the transparency's changing from the current value to totally transparent/visible
        void Set_Transparent(Uint8 alpha_end,Uint32 duration,Uint32 time_lag,Uint32 current_time); //to start the transparency's changing from the current value given to another value given
        virtual void Update_Transparent(Uint32 current_time); //to update the timer current
    protected :
        virtual void transparency(); //to ajust alpha the proper way
    private:
};

#endif // BASIC_TRANSPARENCY_H
