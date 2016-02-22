#ifndef QUIT_H
#define QUIT_H

/**
** Generic class to display specific screen during the games.
** These screens are for example the options, the beginning or the end.
**/
#include "Definitions.h"

#if COMPLETE_GAME > 0
#if defined (__GNUC__)
	#include "SDL/SDL.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
#endif  //SDL include
#endif //#if COMPLETE_GAME > 0

	class Quit //Just contain the method Quit()
	{
	protected:
#if COMPLETE_GAME > 0
		SDL_Event* _event;
#endif //#if COMPLETE_GAME > 0
		Uint8 * _keystates;
		const bool _destruct;
	public:
		Quit();
#if COMPLETE_GAME > 0
		Quit(SDL_Event* pevent);
#endif // #if COMPLETE_GAME > 0
		virtual ~Quit();
		bool KeepLooping();
	private:
	};

#endif // QUIT_H
