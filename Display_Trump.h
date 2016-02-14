#ifndef DISPLAY_TRUNK_H
#define DISPLAY_TRUNK_H

/**
** This class represents the trump (top left of the game).
** It is composed of the image of the trump (heart, club etc.)
** the team that took (1 or 2).
** It can also tell when the mouse if over it. When it is the case,
** the old trick is revealed.
**/

#include <string>
#include <array>

#if COMPLETE_GAME > 0
#if defined (__GNUC__)
	#include "SDL/SDL.h"
	#include "SDL/SDL_ttf.h"
	#include "SDL/SDL_image.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
	#include <SDL_ttf.h>
	#include <SDL_image.h>
#endif //SDL include
#endif //COMPLETE_GAME > 0

namespace SDL
{
#if COMPLETE_GAME > 0
class Display_trump
{
	private:
		SDL_Surface* _surface;
		SDL_Surface* _surface_name;
		SDL_Rect _clip[4];
		SDL_Color _color;
		SDL_Event* _pEvent;
		TTF_Font* _font;
		int _trunk;
		int _taker;
		std::array<std::string, 4> _names;
		int _on_it;

	public:
		Display_trump();
		virtual ~Display_trump();
		void Apply_sprite(SDL_Surface* destination);
		void Set_color(int color);
		void Set_names(std::string name0, std::string name1, std::string name2, std::string name3);
		void Set_taker(int taker);
		void Set_event(SDL_Event* pevent);
		bool Mouse_over() const { return _on_it >= 10; }
		void Update();
	protected:
		SDL_Surface* load_sprite(std::string filename);
		Display_trump(const Display_trump& other);
		Display_trump& operator=(const Display_trump& other);
};
#else
	class Display_trump
	{
	private:
		int _trunk;
		int _taker;
		std::array<std::string, 4> _names;
		int _on_it;

	public:
		Display_trump(){}
		virtual ~Display_trump(){}
		void Apply_sprite() {}
		void Set_color(int color){}
		void Set_names(std::string name0, std::string name1, std::string name2, std::string name3) {}
		void Set_taker(int taker) {}
		void Set_event() {}
		bool Mouse_over() const { return _on_it >= 10; }
		void Update() {}
	protected:
		//SDL_Surface* load_sprite(std::string filename);
		Display_trump(const Display_trump& other) {}
		Display_trump& operator=(const Display_trump& other) {}
	};
#endif //COMPLETE_GAME > 0
};
#endif // DISPLAY_TRUNK_H
