#ifndef IMAGES_BUTTON_H
#define IMAGES_BUTTON_H

/**
** This class defines properly what is a button
** an image with 2 faces (depending if the mouse is over or not)
** and a thing we can click on
**/

#include "Images_Sprite.h"
#include "Images_Click.h"
#include "Images_Text.h"

namespace SDL
{
	class Images_Button :public Images_Sprite<2>, public Basic_Click //changes color when mouse is over (or clicked on) and we can click on
	{
		protected:
			Images_Text _text_on;
			Images_Text _text_off;
			bool _persistant;
		public:
			Images_Button() {}
#if COMPLETE_GAME > 0
			Images_Button(SDL_Event* event, std::string text_button, PositionGraphic pos);
			Images_Button(SDL_Event* event, PositionGraphic pos, std::string path, int color);
#else 
			Images_Button(std::string text_button, PositionGraphic pos) {}
			Images_Button(PositionGraphic pos, std::string path, int color) {}
#endif //#if COMPLETE_GAME > 0
			virtual ~Images_Button() {}
			virtual void Update_on_it();
#if COMPLETE_GAME > 0
			virtual void Display(SDL_Surface* destination);
#else
			virtual void Display(){}
#endif //#if COMPLETE_GAME > 0
			virtual void SetPosition(PositionGraphic& pos);

		protected:
			virtual void act();
			void PositionText();
		private:
	};
}
#endif // IMAGES_BUTTON_H
