#ifndef IMAGES_TEXT_H
#define IMAGES_TEXT_H

/**
** This class define an specific type of image :
** an image composed of a text.
**/

#include "Basic_Images.h"
#include "Basic_Text.h"

namespace SDL
{
	class Images_Text : public Basic_Images, public Basic_Text
	{
	public:
		virtual ~Images_Text() {}
#if COMPLETE_GAME > 0
		Images_Text();
		Images_Text(std::string path_font, int size, char colR, char colG, char colB, std::string text);
		virtual void Display(SDL_Surface* destination);
#else 
		Images_Text() {}
		Images_Text(std::string path_font, int size, char colR, char colG, char colB, std::string text) {}
		virtual void Display() {}
#endif //#if COMPLETE_GAME > 0
	};
}
#endif // IMAGES_TEXT_H
