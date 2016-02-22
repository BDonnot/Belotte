#ifndef SCREEN_END_H
#define SCREEN_END_H
/**
** This class handles the displaying of
** the screen at the end, after the player quit.
**/

#include "Quit.h"
#include "Basic_Images.h"
#include "Images_Button.h"
#include "Images_Text.h"
	class Screen_end : public Quit
	{
	public:
		Screen_end() {}
#if COMPLETE_GAME > 0
		Screen_end(Basic_Images* fond, SDL_Event* pevent, double screen_width, double screen_height, std::string message);
		void Display(SDL_Surface* destination);
#endif //#if COMPLETE_GAME > 0
		virtual ~Screen_end() {}
		Screen_end(Basic_Images* fond, double screen_width, double screen_height, std::string message);
		void Display() {}
	protected:
		Basic_Images* _fond;
		Images_Button _quit;
		Images_Text _thanks;
	private:
	};
#endif // SCREEN_END_H
