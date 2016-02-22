#ifndef IMAGES_SPRITEEVENT_H
#define IMAGES_SPRITEEVENT_H

/**
** This class define an specific type of image :
** an image with multiple faces, that can be clicked on.
**/

#include "Definitions.h"

#if COMPLETE_GAME > 0
#if defined(__GNUC__)
	#include "SDL/SDL.h"
	#include "SDL/SDL_image.h"
#elif defined(_MSC_VER)
	#include <SDL.h>
	#include <SDL_image.h>
#endif //SDL include
#endif //#if COMPLETE_GAME > 0

#include "Images_Sprite.h"
#include "Basic_Click.h"
template<Uint numberOfSprite>
class Images_SpriteEvent : public Images_Sprite<numberOfSprite>, public Basic_Click
{
public:
	Images_SpriteEvent() {}
#if COMPLETE_GAME > 0
	Images_SpriteEvent(std::string path, SDL_Event* pevent) :Images_Sprite<numberOfSprite>(path), Basic_Click(pevent) {}
#endif //#if COMPLETE_GAME > 0
	Images_SpriteEvent(std::string path) : Images_Sprite<numberOfSprite>(path), Basic_Click() {}
	virtual ~Images_SpriteEvent() {}
#if COMPLETE_GAME > 0
	virtual void Update_on_it();
#else
	virtual void Update_on_it(){}
#endif //#if COMPLETE_GAME > 0
protected:
private:
};

#if COMPLETE_GAME > 0
template<Uint numberOfSprite>
void Images_SpriteEvent<numberOfSprite>::Update_on_it()
{
	if (_pEvent->type == SDL_MOUSEMOTION)
	{
		Uint pos_mouse_x = _pEvent->motion.x;
		Uint pos_mouse_y = _pEvent->motion.y;
		const Uint& pos_x = (this)->_pos.Getx();
		const Uint& pos_y = (this)->_pos.Gety();
		const Uint& width = (this)->_width;
		const Uint& height = (this)->_height;
		if (_on_it&&!((pos_mouse_x > pos_x) && (pos_mouse_x < pos_x + width) && (pos_mouse_y > pos_y) && (pos_mouse_y < pos_y + height))) _on_it = false;
		else
		{
			if (!_on_it && ((pos_mouse_x > pos_x) && (pos_mouse_x<pos_x + width) && (pos_mouse_y >pos_y) && (pos_mouse_y < pos_y + height))) _on_it = true;
		}
	}
	Set_click_on_long();
	Set_click_on_short();
}
#endif //#if COMPLETE_GAME > 0

#endif // IMAGES_SPRITEEVENT_H
