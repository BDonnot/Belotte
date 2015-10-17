#ifndef QUIT_H
#define QUIT_H

/**
** Generic class to display specific screen during the games.
** These screens are for example the options, the beginning or the end.
**/

#if defined (__GNUC__)
	#include "SDL/SDL.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
#endif  //SDL include

namespace SDL
{
	class Quit //Just contain the method Quit()
	{
	protected:
		SDL_Event* _event;
		Uint8* _keystates;
		const bool _destruct;
	public:
		Quit();
		Quit(SDL_Event* pevent);
		virtual ~Quit();
		bool KeepLooping();
	private:
	};
}
#endif // QUIT_H
