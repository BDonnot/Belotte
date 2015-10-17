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
namespace SDL
{
	class Screen_end : public Quit
	{
	public:
		Screen_end() {}
		Screen_end(Basic_Images* fond, SDL_Event* pevent, double screen_width, double screen_height, std::string message);
		virtual ~Screen_end() {}
		void Display(SDL_Surface* destination);
	protected:
		Basic_Images* _fond;
		Images_Button _quit;
		Images_Text _thanks;
	private:
	};
}
#endif // SCREEN_END_H
