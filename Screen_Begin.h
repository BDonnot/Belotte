#ifndef SCREEN_BEGIN_H
#define SCREEN_BEGIN_H

/**
** This class handles the displaying of
** the screen beginning, when we choose
** between option/play and return
**/

#include "Quit.h"
#include "Screen_Option.h"
#include "Basic_Images.h"
#include "Images_Button.h"

class Screen_Begin : public Quit
	{
	protected:
		Basic_Images* _fond;
		Images_Button _play;
		Images_Button _option;
		Images_Button _quit;
		bool _onScreen;
		Screen_Option _options;
	public:
		Screen_Begin() {}
		Screen_Begin(Basic_Images* fond, double screen_width, double screen_height);

#if COMPLETE_GAME > 0

		Screen_Begin(Basic_Images* fond, SDL_Event* pevent, double screen_width, double screen_height);
		OUTPUT_BEGIN Display(SDL_Surface* destination);

#endif //#if COMPLETE_GAME > 0

		virtual ~Screen_Begin() {}
		OUTPUT_BEGIN Display() { return PLAY; }
		void Update();
	private:
	};
#endif // SCREEN_BEGIN_H
