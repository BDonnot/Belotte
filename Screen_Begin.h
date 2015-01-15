#ifndef SCREEN_BEGIN_H
#define SCREEN_BEGIN_H

#include "Quit.h"
#include "Screen_Option.h"
#include "Basic_Images.h"
#include "Images_Button.h"

class Screen_Begin : public Quit
{
    public:
        Screen_Begin();
        Screen_Begin(Basic_Images* fond,SDL_Event* pevent,double screen_width,double screen_height);
        virtual ~Screen_Begin();
        OUTPUT_BEGIN Display(SDL_Surface* destination);
        void Update();
    protected:
        Basic_Images* _fond;
        Images_Button _play;
        Images_Button _option;
        Images_Button _quit;
        bool _onScreen;
        Screen_Option _options;
    private:
};

#endif // SCREEN_BEGIN_H
