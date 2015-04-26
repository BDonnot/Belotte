#ifndef BASIC_CLICK_H
#define BASIC_CLICK_H

#ifndef _MSC_VER
	#include "SDL/SDL.h"
#endif //ndef _MSC_VER
#ifdef _MSC_VER
	#include <SDL.h>
#endif //def _MSC_VER


class Basic_Click
{
    protected :
        bool _on_it; //if the mouse is over
        bool _click_on_short; //if we click on
        bool _click_on_long; //if we have clicked on, and we still don't have clicked somewhere else
        SDL_Event* _pEvent; //pointer to the current event
    public:
        Basic_Click();
        Basic_Click(SDL_Event* event);
        virtual ~Basic_Click();

        bool Mouse_Over(); //true if the mouse is over the images
        bool Click(bool Short); //return true if we click short /long on the image
        virtual void Update_on_it(); //update the position of the mouse and the click
        SDL_Event* GetEvent();
        void Reset_Click();
    protected:
        void Set_click_on_long(); //we 'long click' on a surface if we clicked on it once, and we doesn't click anywhere else in the mean time
        void Set_click_on_short(); //we 'short click' on a surface if we just cicked on it
    private:
};

#endif // BASIC_CLICK_H
